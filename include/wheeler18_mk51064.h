#pragma once
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define W18_RAM_OFFSET(addr)          ((uint32_t)(addr) & 0x00FFFFFFu)

#define W18_ADDR_CAMERA_UPDATE        0x0C08DDC0u
#define W18_ADDR_COCKPIT_INIT         0x0C08F4E0u
#define W18_PATCH_COCKPIT_FOV_I16     0x0C08F5C4u
#define W18_ADDR_SETUP_PERSPECTIVE    0x0C052AC0u
#define W18_ADDR_BUILD_PROJECTION     0x0C1578A0u
#define W18_ADDR_FRUSTUM_CULL_SPHERE  0x0C13ED80u

#define W18_PTR_MATRIX_MODE0_BASE     0x0C39EDE4u
#define W18_PTR_MATRIX_MODE1_VIEW     0x0C39EDF0u
#define W18_PTR_MATRIX_MODE2_MODEL    0x0C39EDFCu
#define W18_PTR_MATRIX_MODE3_PROJ     0x0C39EE08u

#define W18_ADDR_PROJ_SCALE_X         0x0C217AECu
#define W18_ADDR_PROJ_SCALE_Y         0x0C217AF0u
#define W18_ADDR_FOCAL_X              0x0C39EFF0u
#define W18_ADDR_FOCAL_Y              0x0C39F07Cu
#define W18_ADDR_FRUSTUM_PLANES       0x0C39F038u
#define W18_ADDR_CULL_RADIUS_SCALE    0x0C6F65E8u

#define W18_ADDR_P1_CAMERA            0x0C56A478u
#define W18_ADDR_P1_CONTROLS          0x0C27396Cu

typedef enum W18ViewMode {
    W18_VIEW_BIRD    = 0,
    W18_VIEW_COCKPIT = 1
} W18ViewMode;

typedef struct W18Camera {
    uint16_t viewmode;        /* +0x00: 0 = BIRD, 1 = COCKPIT */
    uint16_t submode;         /* +0x02 */
    uint8_t  pad_04[8];       /* +0x04..+0x0B */
    int32_t  angle_x;         /* +0x0C: Pitch (16-bit BAM) */
    int32_t  angle_y;         /* +0x10: Yaw (16-bit BAM) */
    float    length;          /* +0x14: Chase follow distance */
    int32_t  projection_fov;  /* +0x18: Active interpolated FOV (BAM) */
    float    ofs_x;           /* +0x1C: Camera offset X */
    float    ofs_y;           /* +0x20: Camera offset Y */
    float    ofs_z;           /* +0x24: Camera offset Z */
    uint8_t  pad_28[12];      /* +0x28..+0x33 */
    int32_t  target_fov;      /* +0x34: Target FOV (BAM) */
    uint8_t  reserved[340];   /* +0x38..+0x18B (total 0x18C bytes) */
} W18Camera;

typedef struct W18TruckControls {
    float    handle;          /* 0x0C27396C: Steering (-1.0f to +1.0f) */
    uint32_t unk_70;          /* 0x0C273970 */
    float    accel;           /* 0x0C273974: Gas pedal (0.0f to 1.0f) */
    uint32_t unk_78;          /* 0x0C273978 */
    float    brake_pedal;     /* 0x0C27397C: Brake pedal (0.0f to 1.0f) */
    uint32_t unk_80[3];       /* 0x0C273980..0x0C27398B */
    int32_t  view_btn;        /* 0x0C27398C: View toggle state */
    uint32_t unk_90[3];       /* 0x0C273990..0x0C27399B */
    int32_t  gear;            /* 0x0C27399C: Shifter state (Lo/Hi/Rev) */
    uint32_t unk_A0;          /* 0x0C2739A0 */
    int32_t  horn;            /* 0x0C2739A4: Air horn pull cord */
} W18TruckControls;

#ifdef __cplusplus
static_assert(sizeof(W18Camera) == 0x18C, "W18Camera size must be 0x18C bytes");
static_assert(offsetof(W18Camera, projection_fov) == 0x18, "projection_fov offset mismatch");
static_assert(offsetof(W18Camera, target_fov) == 0x34, "target_fov offset mismatch");
static_assert(offsetof(W18TruckControls, horn) == 0x38, "horn offset mismatch");
}
#endif
