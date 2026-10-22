# 18 Wheeler: American Pro Trucker (Dreamcast) — Decompilation & Meta Quest VR Port

[![Target: Meta Quest VR](https://img.shields.io/badge/Target-Meta_Quest_VR_(OpenXR)-blue)](#standalone-meta-quest-vr-port)
[![Version: USA MK-51064 V1.500](https://img.shields.io/badge/ROM-USA_MK--51064_(V1.500)-green)](#target-binary-specification)
[![Engine: SEGA-AM2 NAOMI/YUKA](https://img.shields.io/badge/Engine-SEGA--AM_NAOMI%26YUKA(G2)_S.52-orange)](#critical-sh-4-architecture-note-p0-base-0x0c010000)

An active reverse-engineering, decompilation, and standalone **Meta Quest 2 / 3 / 3S VR port** project for the Sega Dreamcast version of **18 Wheeler: American Pro Trucker** (Sega AM2, 2001), built using **Reverse Engineer Anything (REA)**, **Ghidra**, and an OpenXR **Flycast** reprojection backend (`mikermak/hotd2-vr`) targeting [QuestPorts](https://questports.vercel.app/).

> **Clean-Room Policy:** This repository contains **zero** copyrighted Sega binaries, ROM tracks, or game assets. Users must supply their own legally dumped USA (`MK-51064`) disc image (`.gdi` or `.chd`).

---

## Target Binary Specification

| Field | Value |
| :--- | :--- |
| **Title** | `18 WHEELER AMERICAN PRO TRUCKER` |
| **Product ID / Version** | `MK-51064` / `V1.500` (USA NTSC 60 Hz, built `2001-04-23`) |
| **Engine ID** | `SEGA-AM NAOMI&YUKA(G2) S.52` |
| **`1ST_READ.BIN` Size** | `2,162,688` bytes |
| **`1ST_READ.BIN` SHA-256** | `f957768b96a71a7fd819874449635069ef669960cd8484f2ef9280d7efa1cd2e` |
| **Execution Base Address** | `0x0C010000` (SH-4 `P0` cached region — mirrored at `0x8C010000` in `P1`) |

---

## Critical SH-4 Architecture Note: `P0` Base (`0x0C010000`)

Although the Dreamcast BIOS loads `1ST_READ.BIN` at `0x8C010000`, the entry point immediately masks off the upper 3 bits (`AND #0x1FFFFFFF`) and jumps into **SH-4 `P0` address space (`0x0C010000`–`0x0C220000`)**. Every pointer inside the binary's literal pools uses `0x0C......`.

Use `tools/prepare_18wheeler_rea.py` to extract `1ST_READ.BIN` from your USA GD-ROM dump and wrap it into `1ST_READ.elf` mapped at **`0x0C010000`** so Ghidra and **Reverse Engineer Anything (REA)** resolve 100% of cross-references automatically.

---

## Progress Tracker

| Subsystem | Status | Key Addresses (`P0`) | Notes |
| :--- | :--- | :--- | :--- |
| **GD-ROM Extractor & REA ELF Wrapper** | Complete | `0x0C010000` | `tools/prepare_18wheeler_rea.py` extracts `1ST_READ.BIN` & `IP.BIN` from Track 3 and wraps ELF for REA. |
| **Master Camera & Cockpit Lock** | Mapped | `0x0C08DDC0` / `0x0C56A478` | Player 1 camera struct (`0x18C` bytes); `+0x00` (`uint16`) locks `1:COCKPIT` mode. |
| **Projection & Frustum Builder** | Mapped | `0x0C052AC0` -> `0x0C1578A0` | Computes both 4x4 Projection Matrix & 4 frustum planes (`0x0C39F038`) from a single FOV angle. |
| **Master Matrix Stack Table** | Mapped | `0x0C39EDDC` | Mode 1 View (`0x0C39EDF0`), Mode 2 Model (`0x0C39EDFC`), Mode 3 Projection (`0x0C39EE08`). |
| **Bounding-Sphere Frustum Culling** | Mapped | `0x0C13ED80` | `ftrv xmtrx, fv0` sphere transform + plane tests against `0x0C39F038`. |
| **Truck Controls (6DoF Hook)** | Mapped | `0x0C27396C`–`0x0C2739A4` | Direct RAM floats/ints for `handle` (wheel), `accel`, `break`, `view`, `gear` (shifter), and `horn` (cord). |
| **Standalone Quest OpenXR Profile** | In Progress | `vr-port/` | Drop-in `wheeler18_vr_profile.hpp` for `mikermak/hotd2-vr`; HUD `1/w` depth tuning next. |

---

## Repository Structure

* [`docs/memory_map.md`](docs/memory_map.md) — Detailed breakdown of `MK-51064` functions, camera structs, matrix pointers, and SH-4 instruction patches.
* [`include/wheeler18_mk51064.h`](include/wheeler18_mk51064.h) — C/C++ structs and address macros for decompilation and VR hooks.
* [`tools/prepare_18wheeler_rea.py`](tools/prepare_18wheeler_rea.py) — Zero-dependency Python script to extract `1ST_READ.BIN` / `IP.BIN` from Track 3 and build `1ST_READ.elf` at `0x0C010000`.
* [`vr-port/wheeler18_vr_profile.hpp`](vr-port/wheeler18_vr_profile.hpp) — Drop-in C++ header for `mikermak/hotd2-vr` implementing Cockpit lock, wide VR FOV, matrix extraction, and 6DoF Touch controller truck controls.
* [`vr-port/INTEGRATION.md`](vr-port/INTEGRATION.md) — Step-by-step guide to compiling `18wheeler-vr.apk` for Meta Quest and QuestPorts.
