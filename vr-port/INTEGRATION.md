# Integrating `18 Wheeler (MK-51064)` into `mikermak/hotd2-vr` for Meta Quest

Follow these steps to compile the standalone Meta Quest APK (`18wheeler-vr.apk`) and Windows OpenXR test build using [`wheeler18_vr_profile.hpp`](wheeler18_vr_profile.hpp).

## 1. Clone the OpenXR Flycast Base

```powershell
git clone --recursive https://github.com/mikermak/hotd2-vr.git 18wheeler-vr
```

Copy `include/wheeler18_mk51064.h` and `vr-port/wheeler18_vr_profile.hpp` into `18wheeler-vr/core/rend/`.

## 2. Register `MK-51064` in `VrGames.java`

Open `shell/android-studio/flycast/src/main/java/com/flycast/emulator/vr/VrGames.java` and add `MK-51064` to the supported game definitions so the Quest setup screen finds the user's legally dumped `.gdi` or `.chd` in `/sdcard/Download/`.

In `shell/android-studio/flycast/build.gradle`, set a unique application ID:

```groovy
defaultConfig {
    applicationId "com.flycast.emulator.vr.wheeler18"
}
```

## 3. Hook `vr_reproject.cpp`

Include `wheeler18_vr_profile.hpp` inside `core/rend/vr_reproject.cpp` and call `Wheeler18VR::applyFramePatches(mem_b.data)` and `Wheeler18VR::readActiveMatrices(mem_b.data, viewMat, projMat)`.

## 4. Build & Test

* **Windows PCVR Test Build:** Run `build-win.cmd`
* **Standalone Meta Quest APK:** Run `build-quest.cmd`
