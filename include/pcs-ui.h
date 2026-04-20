#ifndef PCS_UI_H
#define PCS_UI_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <stdlib.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "pcs-colors.h"
#include "pcs-themes.h"
#include "pcs-version.h"

#define DATA_PATH "./data/"

Theme tron_theme = { "\x1B[38;2;72;162;178m", "\x1B[38;2;77;208;225m", "\x1B[38;2;91;209;150m", "\x1B[38;2;252;116;30m", "\x1b[48;2;2;3;5m\033[2J\033[H"};
Theme monolith_theme = { "\x1B[38;2;109;18;15m", "\x1B[38;2;178;72;72m", "\x1B[38;2;255;255;255m", "\x1b[38;2;255;4;0m", "\x1b[48;2;5;2;2m\x1b[2J\x1b[H"};
Theme solar_lime_theme = { "\x1b[38;2;209;217;192m", "\x1b[38;2;182;255;59m", "\x1b[38;2;125;184;28m", "\x1b[38;2;255;69;0m", "\x1b[48;2;4;9;15m\x1b[2J\x1b[H"};
Theme arctic_slate_theme = { "\x1b[38;2;169;204;232m", "\x1b[38;2;203;213;225m", "\x1b[38;2;0;242;255m", "\x1b[38;2;248;250;252m", "\x1b[48;2;31;41;66m\x1b[2J\x1b[H"};
Theme quantum_plasma_theme = { "\x1b[38;2;143;102;255m", "\x1b[38;2;79;27;255m", "\x1B[38;2;0;255;194m", "\x1B[38;2;255;0;122m", "\x1b[48;2;11;15;55m\x1b[2J\x1b[H"};
Theme void_ember_theme = { "\x1b[38;2;178;75;0m", "\x1b[38;2;255;140;0m", "\x1b[38;2;255;252;0m", "\x1b[38;2;255;23;0m", "\x1b[48;2;11;6;3m\x1b[2J\x1b[H"};
Theme limitless_azure_theme = { "\x1b[38;2;90;100;115m", "\x1b[38;2;0;19;194m", "\x1b[38;2;0;191;99m", "\x1b[38;2;159;18;13m", "\x1b[48;2;244;246;250m\x1b[2J\x1b[H"};
Theme gothic_noir_theme = { "\x1b[38;2;201;122;122m", "\x1b[38;2;123;64;100m", "\x1b[38;2;186;104;200m", "\x1b[38;2;216;27;96m", "\x1b[48;2;23;11;19m\x1b[2J\x1b[H"};
Theme reset = { "\x1B[0m", "\x1B[0m", "\x1B[0m", "\x1B[0m", "\x1b[0m\033[2J\033[H"};

Theme* theme; 

static inline int apply_theme() { 
    int themeID;
    char linetheme[500];

    char *filePath = DATA_PATH "pcs-user-data.txt";

    FILE *t = fopen(filePath, "r");
    if (t == NULL) {
        printf("Error: Could not open file at %s\n", filePath);
    } else {
        while (fgets(linetheme, sizeof(linetheme), t) != NULL) {
            if (strncmp(linetheme, "8|", 2) == 0) {
                sscanf(linetheme, "8| %d", &themeID);
                break;
            }
        }
        fclose(t);
    }

    switch(themeID) {
        case 1: theme = &tron_theme; break;
        case 2: theme = &monolith_theme; break;
        case 3: theme = &solar_lime_theme; break;
        case 4: theme = &arctic_slate_theme; break;
        case 5: theme = &quantum_plasma_theme; break;
        case 6: theme = &void_ember_theme; break;
        case 7: theme = &limitless_azure_theme; break;
        case 8: theme = &gothic_noir_theme; break;
        default: theme = &tron_theme; break;
    }
}
    

static inline void gridhelpcommands () {
    // Header
    printf("\n%s===========================================================================", ACCENT_C);
    printf("\n%s------------------------[SYSTEM-COMMAND-INTERFACE]------------------------- %s", MAIN_C, ACCENT_C);
    printf("\n===========================================================================\n");

    // Task Management Sector
    printf("\n%s [SECTOR 01: TASK MANAGEMENT]%s\n", ACCENT_C, MAIN_C);
    printf("  - pcs add task \"task\"     - pcs delete task       - pcs new task list\n");
    printf("  - pcs resolve task        - pcs reset task        - pcs see task\n");
    printf("  - pcs retrieve task list\n");
    printf("    %s>> ACTIONS: 'cancel'%s\n", ACCENT_C, MAIN_C);

    // Logging Sector
    printf("\n%s [SECTOR 02: DATA LOGGING]%s\n", ACCENT_C, MAIN_C);
    printf("  - pcs new log             - pcs add log           - pcs delete log\n");
    printf("  - pcs see log             - pcs retrieve log\n");
    printf("\n    %s>> ACTIONS: 'cancel'%s\n", ACCENT_C, MAIN_C);

    // Todo & Priority Sector
    printf("\n%s [SECTOR 03: TODO OPERATIONS]%s\n", ACCENT_C, MAIN_C);
    printf("  - pcs add todo \"task\" [prio] (cat)\n");
    printf("  - pcs see todo [prio] (cat) {stat}\n");
    printf("  - pcs delete todo         - pcs purge todo\n");
    printf("    %s>> ACTIONS: 'cancel'%s\n", ACCENT_C, MAIN_C);

    // Detail Legends
    printf("\n    %s>> %sPRIORITIES:%s  %slow%s, %smedium%s, %shigh%s, %surgent%s\n", 
           ACCENT_C, MAIN_C, ACCENT_C, 
           COLOR_STATUS_LOW, MAIN_C, COLOR_STATUS_MEDIUM, MAIN_C, 
           COLOR_STATUS_HIGH, MAIN_C, COLOR_STATUS_URGENT, MAIN_C);

    printf("    %s>> %sCATEGORIES:%s  personal, work, school, fitness\n", 
           ACCENT_C, MAIN_C, MAIN_C);

    printf("    %s>> %sSTATUSES:    %s[+] >> resolved%s, %s[!] >> pending%s\n", 
           ACCENT_C, MAIN_C, SUCCESS_C, MAIN_C, ERR_C, MAIN_C);

    printf("\n%s [SECTOR 04: DATA ARCHIVES]%s\n", ACCENT_C, MAIN_C);
    printf("  - pcs see archives\n");
    printf("  - pcs purge archives\n");
    printf("    %s>> ACTIONS: 'cancel'%s\n", ACCENT_C, MAIN_C);

    // System Sector
    printf("\n%s [SECTOR 05: SYSTEM]%s\n", ACCENT_C, MAIN_C);
    printf("  - pcs user data           - pcs edit user data\n");
    printf("  - pcs themes              - pcs rectif\n");
    printf("  - pcs help                - pcs exit\n");
    printf("  - pcs repair              - pcs flush\n");
    printf("    %s>> ACTIONS: 'cancel', 'next', 'prev'%s\n", ACCENT_C, MAIN_C);

    printf("%s===========================================================================\n%s", ACCENT_C, MAIN_C);
}

static inline void show_theme_menu() {
    // Header
    printf("\n%s===========================================================================", ACCENT_C);
    printf("\n%s-----------------------[SYSTEM-ENVIRONMENT-OVERRIDE]----------------------- %s", MAIN_C, ACCENT_C);
    printf("\n===========================================================================\n");

    // Theme Manifest Sector
    printf("\n%s [SECTOR 06: THEME MANIFEST]%s\n", ACCENT_C, MAIN_C);
    
    // Display each theme with its ID and a small color preview
    printf("  ID: 1 | %-15s [DEFAULT]  >>  %sCYAN / NEON%s\n", "TRON", tron_theme.accent_color, MAIN_C);
    printf("  ID: 2 | %-15s            >>  %sCRIMSON / DARK%s\n", "MONOLITH", monolith_theme.accent_color, MAIN_C);
    printf("  ID: 3 | %-15s            >>  %sLIME / SOLAR%s\n", "SOLAR LIME", solar_lime_theme.accent_color, MAIN_C);
    printf("  ID: 4 | %-15s            >>  %sICE / SLATE%s\n", "ARCTIC SLATE", arctic_slate_theme.accent_color, MAIN_C);
    printf("  ID: 5 | %-15s            >>  %sPLASMA / ELECTRIC%s\n", "QUANTUM PLASMA", quantum_plasma_theme.accent_color, MAIN_C);
    printf("  ID: 6 | %-15s            >>  %sEMBER / COAL%s\n", "VOID EMBER", void_ember_theme.accent_color, MAIN_C);
    printf("  ID: 7 | %-15s            >>  %sAZURE / LIGHT%s\n", "LIMITLESS AZURE", limitless_azure_theme.accent_color, MAIN_C);
    printf("  ID: 8 | %-15s            >>  %sAUBERGINE / ROSE%s\n", "GOTHIC NOIR", gothic_noir_theme.accent_color, MAIN_C);
    printf("  ID: 0 | [ABORT SYSTEM ENVIRONMENT OVERRIDE]  %s\n", ACCENT_C);

    printf("\n%s >> Select target theme ID: %s", ACCENT_C, MAIN_C);
}

static inline void show_archives_menu() {
					// Header
					printf("\n%s===========================================================================", ACCENT_C);
					printf("\n%s-------------------------[ARCHIVE-SECTOR-ACCESS]-------------------------- %s", MAIN_C, ACCENT_C);
					printf("\n===========================================================================\n");

					// Archive Selection Sector
					printf("\n%s [SECTOR 04: DATA ARCHIVES]%s\n", ACCENT_C, MAIN_C);
					
					printf("  ID: 1 | %-15s >>  TASK ARCHIVES\n", MAIN_C);
					printf("  ID: 2 | %-15s >>  LOG ARCHIVES\n\n", MAIN_C);
                    printf("  ID: 0 | [ABORT ARCHIVE SECTOR ACTION]  %s\n", ACCENT_C);

					printf("\n%s >> SELECT ARCHIVE ID: %s", ACCENT_C, MAIN_C);
				}

static inline void wait_seconds(double seconds) {
    #ifdef _WIN32
        Sleep(seconds * 1000); 
    #else
        sleep(seconds);
    #endif
}

static inline void clear_text() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

static inline void show_loading_short() {
    printf("Accessing Memory Sectors");
    wait_seconds(0.2);
    printf(".");
    wait_seconds(0.2);
    printf(".");
    wait_seconds(0.2);
    printf(".\n");
    wait_seconds(0.2);
}

static inline void show_loading_long() {
    printf("Accessing Memory Sectors");
    wait_seconds(0.5);
    printf(".");
    wait_seconds(0.5);
    printf(".");
    wait_seconds(0.5);
    printf(".\n");
    wait_seconds(0.5);
}

static inline void show_cmd_prmt_load() {
    wait_seconds(0.35);
    printf("%s" "==============================================================\n", ACCENT_C);
    wait_seconds(0.35);
    printf( "  ██████╗  ██████╗ ███████╗\n" );
    wait_seconds(0.35);
    printf( "  ██╔══██╗██╔════╝ ██╔════╝\n" );
    wait_seconds(0.35);
    printf( "  ██████╔╝██║      ███████╗\n" );
    wait_seconds(0.35);
    printf( "  ██╔═══╝ ██║      ╚════██║\n" );
    wait_seconds(0.35);
    printf( "  ██║     ╚██████╗ ███████║\n" );
    wait_seconds(0.35);
    printf("%s" "  ╚═╝      ╚═════╝ ╚══════╝\n"  "%s", ACCENT_C, MAIN_C);
    wait_seconds(0.35);
    printf(" PCS " "%s" "[Version %s]" "%s"" - " "%s" "COREX INITIALIZED  " "%s""          \n", ERR_C, VERSION_STR, MAIN_C, SUCCESS_C, MAIN_C);
    wait_seconds(0.35);
    printf(" (c) 2025 AccelX Industries. " "%s" "Terminal Link Established." "%s"" \n", SUCCESS_C, MAIN_C);
    wait_seconds(0.35);
    printf(" Status: " "%s" "OPTIMAL" "%s"" | Encryption: ""%s""AXL-320" "%s""           \n", SUCCESS_C, MAIN_C, ACCENT_C, MAIN_C);
    wait_seconds(0.35);
    printf("%s" "==============================================================\n"  "%s", ACCENT_C, MAIN_C);
}

static inline void xpress_show_cmd_prmt_load() {
    wait_seconds(0.35);
    printf("%s" "==============================================================\n", ACCENT_C);
    printf( "  ██████╗  ██████╗ ███████╗\n" );
    printf( "  ██╔══██╗██╔════╝ ██╔════╝\n" );
    printf( "  ██████╔╝██║      ███████╗\n" );
    printf( "  ██╔═══╝ ██║      ╚════██║\n" );
    printf( "  ██║     ╚██████╗ ███████║\n" );
    printf("%s" "  ╚═╝      ╚═════╝ ╚══════╝\n"  "%s", ACCENT_C, MAIN_C);
    printf(" PCS " "%s" "[Version %s]" "%s"" - " "%s" "COREX INITIALIZED  " "%s""          \n", ERR_C, VERSION_STR, MAIN_C, SUCCESS_C, MAIN_C);
    printf(" (c) 2025 AccelX Industries. " "%s" "Terminal Link Established." "%s"" \n", SUCCESS_C, MAIN_C);
    printf(" Status: " "%s" "OPTIMAL" "%s"" | Encryption: ""%s""AXL-320" "%s""           \n", SUCCESS_C, MAIN_C, ACCENT_C, MAIN_C);
    printf("%s" "==============================================================\n"  "%s", ACCENT_C, MAIN_C);
}

static inline void showdots(){
    wait_seconds(0.65);
    printf(".");
    wait_seconds(0.65);
    printf(".");
    wait_seconds(0.65);
    printf(".");
}

static inline void to_uppercase(char *s) {
    while (*s) {
        *s = toupper((unsigned char)*s);
        s++;
    }
}

#endif