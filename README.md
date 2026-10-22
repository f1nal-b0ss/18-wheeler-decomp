# 18 Wheeler: American Pro Trucker - Reverse Engineering & Decompilation

Welcome to the central documentation and research hub for the reverse engineering of **18 Wheeler: American Pro Trucker**. This project aims to document file formats, memory structures, game engine logic, and custom tools to better understand and preserve the game.

---

## 🚀 Project Goals

* **File Format Analysis:** Documenting custom asset containers, textures, models, and audio formats.
* **Memory & Struct Mapping:** Identifying entity structures, player physics parameters, and game state variables.
* **Tooling:** Developing custom Python/C++ scripts to parse, extract, and convert game assets.
* **Decompilation Progress:** Working alongside AI assistance to analyze disassembly and translate functions into readable pseudo-code.

---

## 📊 Progress Tracker

| Category | Target / Component | Status | Notes |
| :--- | :--- | :--- | :--- |
| **Assets** | Model / Mesh Formats | 🟡 In Progress | Identifying vertex buffer layouts |
| **Assets** | Texture / Image Parsing | 🔴 Not Started | Looking for header magic bytes |
| **Engine** | Player Physics Struct | 🟢 Completed | Speed, steering angles, and weight offsets mapped |
| **Engine** | Collision Detection | 🔴 Not Started | Pending memory breakpoint analysis |

*(Status: 🟢 Completed, 🟡 In Progress, 🔴 Not Started)*

---

## 🛠️ Tools & Environment

* **Disassembler / Decompiler:** Ghidra / IDA Pro
* **Memory Analysis:** Cheat Engine / x64dbg
* **Scripting:** Python 3.x (with `struct` and `numpy` for binary parsing)

---

## 🤝 Contributing & Collaboration

This project is a collaborative research effort. If you have insights, tools, or corrections regarding the game's architecture, feel free to open an issue or submit a pull request!
