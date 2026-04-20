#ifndef COLORS_H
#define COLORS_H

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// --- SHARED ---
#define COLOR_ACCELX_WHITE    "\x1B[38;2;255;255;255m"
#define COLOR_RESET           "\x1B[0m"
#define CLEAR_ALL             "\x1B[2J\x1B[H"

// --- TRON AESTHETIC ---
#define COLOR_TRON_LBLUE      "\x1B[38;2;72;162;178m"
#define COLOR_TRON_ICE        "\x1B[38;2;77;208;225m"
#define COLOR_TRON_ORANGE     "\x1B[38;2;252;116;30m"   //medium
#define COLOR_TRON_GREEN      "\x1B[38;2;91;209;150m"
#define COLOR_TRON_DBLUE_BG   "\x1B[48;2;2;3;5m"
#define COLOR_TRON_DEEP_TEAL  "\x1B[38;2;41;71;171m"    //low
#define COLOR_TRON_YELLOW       "\x1B[38;2;250;195;33m" //high

// --- MONOLITH AESTHETIC ---
#define COLOR_GM_DLRED_TXT    "\x1B[38;2;109;18;15m"
#define COLOR_GOD_MODE_RED    "\x1B[38;2;178;72;72m"    //urgent
#define COLOR_GM_DRED_BG      "\x1B[48;2;5;2;2m"

// --- TODO LIST COLORS ---
#define COLOR_STATUS_LOW "\x1B[38;2;41;71;171m" //low
#define COLOR_STATUS_MEDIUM "\x1B[38;2;252;116;30m" //medium
#define COLOR_STATUS_HIGH "\x1B[38;2;250;195;33m" //high
#define COLOR_STATUS_URGENT "\x1B[38;2;178;72;72m" //urgent
#define COLOR_STATUS_RESOLVED "\x1B[38;2;91;209;150m" //complete
#define COLOR_STATUS_PENDING "\x1B[38;2;252;116;30m" //pending

#endif