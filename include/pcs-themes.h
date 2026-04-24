#ifndef THEMES_H
#define THEMES_H

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// --- SHARED ---
#define COLOR_RESET           "\x1B[0m"
#define CLEAR_ALL             "\x1B[2J\x1B[H"

// --- TODO LIST COLORS ---
#define COLOR_STATUS_LOW "\x1B[38;2;41;71;171m" //low
#define COLOR_STATUS_MEDIUM "\x1B[38;2;252;116;30m" //medium
#define COLOR_STATUS_HIGH "\x1B[38;2;250;195;33m" //high
#define COLOR_STATUS_URGENT "\x1B[38;2;178;72;72m" //urgent

// Macros for easy access
#define MAIN_C    theme->main_color
#define ACCENT_C  theme->accent_color
#define SUCCESS_C theme->success_color
#define ERR_C     theme->error_color

// The Structure definition
typedef struct {
    const char* main_color;
    const char* accent_color;
    const char* success_color;
    const char* error_color;
    const char* background_color;
} Theme;

// 'extern' tells the compiler these variables exist, 
// but are defined in a different (.c) file.
extern Theme tron_theme;
extern Theme monolith_theme;
extern Theme solar_lime_theme;
extern Theme arctic_slate_theme;
extern Theme quantum_plasma_theme;
extern Theme void_ember_theme;
extern Theme limitless_azure_theme;
extern Theme gothic_noir_theme;
extern Theme reset;

// Permanent memory spots
static char custom_main[40], custom_accent[40], custom_success[40], custom_error[40], custom_bg[40];
Theme user_configured_theme = { 
    custom_main, 
    custom_accent, 
    custom_success, 
    custom_error, 
    custom_bg 
};

#endif