<div align="center">

<img width="3168" height="1344" alt="18WheelerVR_banner" src="https://github.com/user-attachments/assets/e39cea77-d53c-424e-a4be-0b7887aac98b" />

# 18 Wheeler: American Pro Trucker VR
### Standalone 6DoF Meta Quest Port & Dreamcast SH-4 Reverse-Engineering Project

[![Release: v0.1.0](https://img.shields.io/badge/Release-v0.1.0_(Meta_Quest_APK)-brightgreen?style=for-the-badge)](https://github.com/f1nal-b0ss/18-wheeler-decomp/releases)
[![Platform: Meta Quest 2 / Pro / 3 / 3S](https://img.shields.io/badge/Platform-Meta_Quest_Standalone_(OpenXR)-blue?style=for-the-badge)](#installation-instructions-meta-quest)
[![ROM: USA MK-51064 V1.500](https://img.shields.io/badge/Target_Disc-USA_MK--51064_(60_Hz)-orange?style=for-the-badge)](#target-binary-specification)

**[⬇️ Download Latest Quest APK (`v0.1.0`)](https://github.com/f1nal-b0ss/18-wheeler-decomp/releases)** • **[📖 SH-4 Memory & Function Map](docs/memory_map.md)** • **[🌐 QuestPorts Catalog](https://questports.vercel.app/)**

</div>

---

## 🚛 Overview

**18 Wheeler: American Pro Trucker VR** brings Sega AM2's 2001 Dreamcast arcade classic into **standalone 6DoF Virtual Reality** on Meta Quest 2, Quest Pro, Quest 3, and Quest 3S.

Built by reverse-engineering the retail USA SH-4 executable (`1ST_READ.BIN` at `P0` base `0x0C010000`) with **Reverse Engineer Anything (REA)** and **Ghidra**, and extending the [**`mikermak/hotd2-vr`**](https://github.com/mikermak/hotd2-vr) OpenXR Flycast reprojection engine, this port puts you right inside the 3D big-rig cabin with motion controls, overhead air-horn gestures, and real-time haptic telemetry.

> **⚖️ Clean-Room Policy:** This repository and its APK releases contain **zero** copyrighted Sega ROM data, binaries, or game assets. You must provide your own legally dumped **USA (`MK-51064`)** Dreamcast disc image (`.gdi` + `.bin` tracks, or `.chd`).

---

## ✨ Key VR Features

* **Full 6DoF Stereoscopic 3D Truck Cabin:** Look around the 3D cab interior—complete with the wooden dashboard, working speedometer/tachometer gauges, sunglasses, magazine, and swinging windshield badge—powered by a custom **planar-depth cab vertex shader** that eliminates cone distortion and polygon tearing.
* **Physical Overhead Air-Horn Gesture:** Reach your **Left Hand** up near visor level (just like yanking a real 18-wheeler air-horn cord!) or press **Left `Y`** to blast the horn and scatter traffic out of your lane.
* **Dual Steering Modes (Thumbstick or 6DoF Virtual Wheel):** Steer with the analog thumbstick, or squeeze **both Left & Right Grips** to grab a 6DoF virtual steering wheel in mid-air.
* **Multi-Layered Big-Rig Haptics:**
  * **Diesel Idle & Throttle Rumble:** Feel the engine chug at idle and ramp up smoothly as you squeeze the Right Trigger (Gas), plus brake shudder on the Left Trigger.
  * **Mechanical Gear-Shift Clunk:** Tactile right-hand clunk when shifting between **LOW**, **HIGH**, and **REVERSE** gears.
  * **Crash & Collision Jolts:** Live RAM telemetry (`0x8C56A494`–`0x8C56A49C`) triggers heavy dual-controller impact jolts when trading paint with rival **Lizard Tail**, smashing through crates, or hitting guardrails.
* **Arcade CRT Color & Specular Pass:** Custom GLSL shader pass tames LCD vertex-specular washout and restores rich Dreamcast CRT contrast at a locked 60/72 FPS.

---

## 🎮 Meta Quest Touch Controller Layout

| Truck / Menu Action | Quest Touch Controller Input |
| :--- | :--- |
| **Accelerate (Gas Pedal)** | **Right Trigger** (Analog) |
| **Brake / Reverse Pedal** | **Left Trigger** (Analog) |
| **Steer (Standard)** | **Left or Right Thumbstick** (Horizontal) |
| **Steer (6DoF Virtual Wheel)** | Hold **Both Left + Right Grips** and turn your hands like a wheel |
| **Shift Gear Up (`LOW -> HIGH`) / Confirm** | **Right `A` Button** |
| **Shift Gear Down (`HIGH -> LOW -> R`) / Back** | **Right `B` Button** |
| **Blow Air Horn** | **Raise Left Hand to Visor Level** *or* press **Left `Y` Button** |
| **Toggle Camera View (Cab / Bumper / Chase)** | **Right Thumbstick Click** |
| **Start / Pause / Recenter View** | **Left `X` Button** *or* **Left `Menu` (`☰`) Button** |
| **Live Screen Distance Zoom** | Hold **1 Grip** (with stick centered) + push **Thumbstick Left / Right** |
| **Live 3D World Scale Adjust** | Hold **1 Grip** (with stick centered) + push **Thumbstick Up / Down** |

---

## 📦 Installation Instructions (Meta Quest)

### 1. Install the APK
Download **`18wheeler-vr.apk`** from the [**Releases**](https://github.com/f1nal-b0ss/18-wheeler-decomp/releases) page and install it via **[QuestPorts](https://questports.vercel.app/)**, **SideQuest**, or ADB:
```bash
adb install -r 18wheeler-vr.apk
adb shell appops set com.flycast.emulator.wheeler18 MANAGE_EXTERNAL_STORAGE allow
