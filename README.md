<div align="center">

# 🟢 ACCELX://PERSONAL COMMAND SYSTEM (PCS)
**[VERSION 1.1.1] — CORE INITIALIZED**

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

## 🚀 SYSTEM ENHANCEMENTS [PCS-CLI v1.1.1 | The Stability Protocol]

- **Grid Shielding (Buffer Overload Mitigation):** Implemented a 50-to-100-character input ceiling and automatic buffer flushing to prevent memory corruption and "Bit-Stream" loops.

- **Protocol Feedback:** Synchronized terminal audio-visual triggers; resolved an inconsistency where specific syntax errors failed to trigger the standard \a (alert) tone.

- **Data Integrity:** Optimized file-handling architecture to ensure pcs-user-data.txt remains uncorrupted during high-frequency write operations.

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

| Theme            | Vibe                  | Key Colors              |
|------------------|----------------------|--------------------------|
| Tron             | Classic Grid         | Cyan / Neon Orange       |
| Monolith         | Tactical Brutalism   | Deep Red / White         |
| Solar Lime       | High-Visibility      | Lime / Charcoal          |
| Arctic Slate     | Cool / Professional  | Ice Blue / Slate         |
| Quantum Plasma   | Ionized Energy       | Plasma Violet / Mint     |
| Void Ember       | Dying Star           | Deep Charcoal / Ember Orange |
| Limitless Azure  | Hyper-Sky            | Electric Blue / White    |
| Gothic Noir      | Digital Cathedral    | Wine Purple / Crimson    |

---

## 📊 DATA FORMAT

```bash
1| 1 | [!] | [LOW] | (PERSONAL) |
|__________||___________________|
   Tasks      Persistent Tasks

- [!] = pending  
- [+] = complete
```

---

## ⌨️ COMMANDS

```bash
pcs add todo "task" [PRIORITY] (CATEGORY)
pcs see todo [FILTERS]
pcs repair

Example:
pcs add todo "Finish report" [HIGH] (WORK)
pcs see todo [HIGH]
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
├── 📂 assets/      # Icons & Resource Scripts
├── 📂 build/       # Binary Output (.exe)
│   └── 📂 data/    # Persistent Text Storage
├── 📂 include/     # System Headers (.h)
└── 📂 src/         # Core Logic (.c)
```

---

## 📝 DESIGN NOTES

- **Text-based storage for simplicity and transparency**

- **Separate daily vs persistent systems**

- **Repair system ensures zero manual setup**

- **ANSI colors improve CLI readability**

---

## 🛑 LIMITATIONS

- **CLI only**

- **No database (text-based storage)**

- **Basic validation for corrupted entries**

---

## 🧩 FUTURE WORK

- **Sorting and search**

- **Configurable themes**

- **Stronger data validation**

---

## 👤 AUTHOR

Built as a systems-oriented C project focused on command design, persistence, and reliability.

---