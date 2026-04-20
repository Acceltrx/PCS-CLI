#ifndef THEMES_H
#define THEMES_H


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

#endif