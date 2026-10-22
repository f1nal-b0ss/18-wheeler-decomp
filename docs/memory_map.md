# 18 Wheeler: American Pro Trucker (USA `MK-51064`, V1.500) Memory & Function Map

This document records the exact SH-4 code addresses, RAM structures, and instruction patch sites decoded from the retail USA Dreamcast executable (`1ST_READ.BIN`, SHA-256 `f957768b96a71a7fd819874449635069ef669960cd8484f2ef9280d7efa1cd2e`).

## 1. Address Space Convention (`P0` vs. `P1`)

* **Executable Link Base (`P0`):** `0x0C010000` – `0x0C220000`
* **Physical RAM Mirror (`P1`):** `0x8C010000` – `0x8CFFFFFF`
* At boot (`0x8C010000`), the startup stub masks the program counter with `0x1FFFFFFF` and jumps into `0x0C010000`.
* Inside **Flycast / `mikermak/hotd2-vr`**, masking any address with `& 0x00FFFFFF` yields the exact 16 MB Dreamcast System RAM offset (`0x000000`–`0xFFFFFF`).

## 2. Camera System & Cockpit Mode (`0x0C56A478`)

| Offset | RAM Address (P0) | Type | Debug Name | Description |
| :--- | :--- | :--- | :--- | :--- |
| `+0x00` | `0x0C56A478` | `uint16_t` | `viewmode` | `0` = `BIRD` (chase), `1` = `COCKPIT` (1st-person cabin/bumper) |
| `+0x02` | `0x0C56A47A` | `uint16_t` | `submode` | Camera transition / state flags |
| `+0x0C` | `0x0C56A484` | `int32_t` | `angle X` | Camera pitch angle (16-bit BAM) |
| `+0x10` | `0x0C56A488` | `int32_t` | `angle Y` | Camera yaw angle (16-bit BAM) |
| `+0x14` | `0x0C56A48C` | `float` | `length` | Chase camera follow distance |
| `+0x18` | `0x0C56A490` | `int32_t` | `projection` | **Active interpolated FOV** (16-bit BAM angle passed to `0x0C08CBC0`) |
| `+0x1C` | `0x0C56A494` | `float` | `ofs X` | Camera offset X |
| `+0x20` | `0x0C56A498` | `float` | `ofs Y` | Camera offset Y |
| `+0x24` | `0x0C56A49C` | `float` | `ofs Z` | Camera offset Z |
| `+0x34` | `0x0C56A4AC` | `int32_t` | `target_fov` | **Target FOV** (interpolated toward by `0x0C56A490`) |

### Key Camera Functions & FOV Literals
* **`0x0C08DDC0` (`Camera_MasterUpdate`):** Per-frame camera dispatcher.
* **`0x0C08F4E0` (`Camera_InitCockpitMode`):** Loads 16-bit FOV literal at **`0x0C08F5C4`** (`0x1388` = `5000` BAM) and stores it into `0x0C56A4AC` (`+0x34`). Patching `0x0C08F5C4` to `0x3400` (`13312` BAM ≈ 73.1 deg) widens both the projection matrix and frustum planes.

## 3. Projection Matrix & Frustum Plane Builder (`0x0C1578A0`)

* **`0x0C08CBC0` -> `0x0C052AC0` -> `0x0C1578A0` (`njSetPerspective_AM2`):**
  * Writes projection X/Y scale factors to **`0x0C217AEC`** and **`0x0C217AF0`**.
  * Writes screen-space focal lengths to **`0x0C39EFF0`** (`fx`) and **`0x0C39F07C`** (`fy`).
  * Writes the 4 frustum clipping plane normals to **`0x0C39F038`**.
  * Writes the 4x4 Projection Matrix to **`*(uint32_t*)0x0C39EE08`**.

## 4. Master Matrix Stack Table (`0x0C39EDDC`)

| Mode | Current Matrix Ptr Address | Role |
| :--- | :--- | :--- |
| **Mode 0** | `0x0C39EDE4` (`+0x08`) | Base / System Matrix pointer |
| **Mode 1** | `0x0C39EDF0` (`+0x14`) | **Camera View Matrix pointer** (`float[16]`) |
| **Mode 2** | `0x0C39EDFC` (`+0x20`) | **Object / World Model Matrix pointer** (`float[16]`) |
| **Mode 3** | `0x0C39EE08` (`+0x2C`) | **Projection Matrix pointer** (`float[16]`) |

## 5. Truck Controls & Input State (`0x0C27396C` – `0x0C2739A4`)

| RAM Address | Type | Debug Label | Value Range & VR Mapping |
| :--- | :--- | :--- | :--- |
| `0x0C27396C` | `float` | `handle %f` | Steering wheel angle (`-1.0f` full left to `+1.0f` full right) |
| `0x0C273974` | `float` | `accel %f` | Gas pedal (`0.0f` to `1.0f`) — Right Trigger |
| `0x0C27397C` | `float` | `break %f` | Brake pedal (`0.0f` to `1.0f`) — Left Trigger |
| `0x0C27398C` | `int32_t` | `view %d` | Camera view toggle button state |
| `0x0C27399C` | `int32_t` | `gear %d` | Transmission state (Low / High / Reverse) — Virtual right-hand shifter |
| `0x0C2739A4` | `int32_t` | `horn %d` | Air horn state (`0` = off, `1` = blowing) — Overhead left-hand pull cord |
