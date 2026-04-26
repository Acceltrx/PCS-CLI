<div align="center">

# 🟢 ACCELX://PERSONAL COMMAND SYSTEM (PCS)
**[VERSION 1.2.1] — CORE INITIALIZED**

</div>

> [!IMPORTANT]
> **BOOT SEQUENCE INITIATED...**
> 
> `[LOADING CORE MODULES...]`
>
> `[LINKING ACCELX GRID...]`
>
> `[STATUS: ONLINE]`

---

### 🌌 SYSTEM OVERVIEW
**A CLI-based control layer for tasks, logs, and persistent objectives with built-in recovery.**

Built for speed, reliability, and high-contrast terminal aesthetics. Designed by **AccelX Industries**.

---

## 🚀 SYSTEM ENHANCEMENTS [PCS-CLI v1.2.1 | The Refactor & Personalization Update]

- **Core Architecture:** Compacted logic across modules for faster execution and smaller binary size.

- **New Features:** Hex-Theme Engine: Full support for custom #RRGGBB themes via pcs-themes.txt.

---

## ✨ FEATURES

- **Task System (Daily)**  
  Reset-based system with automatic archiving and completion tracking

- **To-do System (Persistent)**  
  Supports priorities, categories, and filtering

- **Repair System**  
  Recreates missing files and restores user data

- **ANSI UI**  
  Color-coded priorities, theme switching, and readable CLI output

**Priorities:** `LOW`, `MEDIUM`, `HIGH`, `URGENT`  
**Categories:** `WORK`, `SCHOOL`, `FITNESS`, `PERSONAL`

---

## 🎨 THEMES

| Theme            | Vibe                 | Key Colors               |
|------------------|----------------------|--------------------------|
| Tron             | Classic Grid         | Cyan / Neon Orange       |
| Monolith         | Tactical Brutalism   | Deep Red / White         |
| Solar Lime       | High-Visibility      | Lime / Charcoal          |
| Arctic Slate     | Cool / Professional  | Ice Blue / Slate         |
| Quantum Plasma   | Ionized Energy       | Plasma Violet / Mint     |
| Void Ember       | Dying Star           | Deep Charcoal / Ember Orange |
| Limitless Azure  | Hyper-Sky            | Electric Blue / White    |
| Gothic Noir      | Digital Cathedral    | Wine Purple / Crimson    |
| Custom Theme	   | Your Own Vibe!       | Your Own Colors!         |

---

## ⌨️ Usage Examples

```bash
pcs add todo "task" [PRIORITY] (CATEGORY)
pcs see todo [FILTERS (Optional: [PRIORITY] (CATEGORY) {STATUS})
pcs repair

Example:
pcs add todo "Finish report" [HIGH] (WORK)
pcs see todo [HIGH]

Hint: Use 'pcs help' to show list of available commands
```

---

## 🖥️ SYSTEM REQUIREMENTS

To ensure the **ACCELX** environment maps correctly and compiles without sector faults, the following specifications are required:

### **1. Core Environment**
* **Operating System:** Windows 10 / 11 (64-bit recommended).
* **Terminal:** [MSYS2](https://www.msys2.org/) with the **MinGW64** environment.
* **Shell:** Bash (via MSYS2) or Windows Terminal (for optimal ANSI color rendering).

### **2. Toolchain (Compiler Suite)**
The system requires the `mingw-w64-x86_64` toolchain. You can initialize these dependencies via the MSYS2 terminal:
```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-make mingw-w64-x86_64-binutils
```

---

## 🛠️ BUILD & RUN

MSYS2 MinGW64
```bash
mkdir -p /c/dev/GitHub

~ extract "PCS-CLI-main.zip", open the folder and move "PCS-CLI-main" folder to "C:\dev\GitHub"

cd /c/dev/github/project_PCS
make or mingw32-make
```

First Run
```bash
"pcs repair"
```

---

## 🏛️ PROJECT STRUCTURE

```bash
📁 PCS-CLI-main
├── 📂 assets/		# Icons & Resource Scripts
├── 📂 build/		# Binary Output (.exe)
│    └── 📂 data/	# Persistent Text Storage
├── 📂 include/	# System Headers (.h)
└── 📂 src/		# Core Logic (.c)
```

---