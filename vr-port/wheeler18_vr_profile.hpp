#pragma once
#include <cstdint>
#include <cstring>
#include <cmath>
#include "../include/wheeler18_mk51064.h"

namespace Wheeler18VR {

template <typename T>
inline T* dc_ram_ptr(uint8_t* ram_base, uint32_t sh4_addr) {
    if (!ram_base) return nullptr;
    uint32_t offset = W18_RAM_OFFSET(sh4_addr);
    if (offset + sizeof(T) > 0x01000000u) return nullptr;
    return reinterpret_cast<T*>(ram_base + offset);
}

inline void applyFramePatches(uint8_t* ram_base, bool force_cockpit = true, int16_t vr_fov_bam = 0x3400) {
    if (!ram_base) return;
    if (auto* fov_lit = dc_ram_ptr<int16_t>(ram_base, W18_PATCH_COCKPIT_FOV_I16)) {
        if (*fov_lit == 0x1388 || *fov_lit != vr_fov_bam) {
            *fov_lit = vr_fov_bam;
        }
    }
    if (auto* cam = dc_ram_ptr<W18Camera>(ram_base, W18_ADDR_P1_CAMERA)) {
        if (force_cockpit && cam->viewmode == W18_VIEW_BIRD) {
            cam->viewmode = W18_VIEW_COCKPIT;
        }
        if (cam->projection_fov > 0 && cam->projection_fov < vr_fov_bam) {
            cam->projection_fov = vr_fov_bam;
            cam->target_fov     = vr_fov_bam;
        }
    }
}

inline bool readActiveMatrices(uint8_t* ram_base, float out_view[16], float out_proj[16]) {
    if (!ram_base) return false;
    auto* view_ptr_addr = dc_ram_ptr<uint32_t>(ram_base, W18_PTR_MATRIX_MODE1_VIEW);
    auto* proj_ptr_addr = dc_ram_ptr<uint32_t>(ram_base, W18_PTR_MATRIX_MODE3_PROJ);
    if (!view_ptr_addr || !proj_ptr_addr) return false;
    uint32_t view_sh4 = *view_ptr_addr;
    uint32_t proj_sh4 = *proj_ptr_addr;
    if ((view_sh4 & 0x0C000000u) != 0x0C000000u || (proj_sh4 & 0x0C000000u) != 0x0C000000u) {
        return false;
    }
    auto* view_mat = dc_ram_ptr<float>(ram_base, view_sh4);
    auto* proj_mat = dc_ram_ptr<float>(ram_base, proj_sh4);
    if (!view_mat || !proj_mat) return false;
    std::memcpy(out_view, view_mat, sizeof(float) * 16);
    std::memcpy(out_proj, proj_mat, sizeof(float) * 16);
    return true;
}

inline void injectVirtualCabControls(
    uint8_t* ram_base,
    bool left_grip, const float left_pos[3],
    bool right_grip, const float right_pos[3],
    float right_trigger, float left_trigger,
    bool horn_pull_gesture)
{
    auto* ctrl = dc_ram_ptr<W18TruckControls>(ram_base, W18_ADDR_P1_CONTROLS);
    if (!ctrl) return;
    if (left_grip && right_grip) {
        float dy = right_pos[1] - left_pos[1];
        float dx = right_pos[0] - left_pos[0];
        float wheel_angle = std::atan2(dy, dx);
        float norm_steer = std::fmax(-1.0f, std::fmin(1.0f, -wheel_angle / 1.05f));
        ctrl->handle = norm_steer;
    }
    ctrl->accel       = right_trigger;
    ctrl->brake_pedal = left_trigger;
    if (horn_pull_gesture) {
        ctrl->horn = 1;
    }
}

} // namespace Wheeler18VR
