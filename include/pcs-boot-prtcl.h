#ifndef PCS_BOOT_PRTCL_H
#define PCS_BOOT_PRTCL_H

#include "pcs-colors.h"
#include "pcs-version.h"
#include "pcs-ui.h"
#include "pcs-themes.h"
#include "pcs-data-types.h"
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

#define DATA_PATH "./data/"

static inline void generate_hex_id(char *dest) {
    static int seeded = 0;
    if (!seeded) {
        srand((unsigned int)time(NULL));
        seeded = 1;
    }
    int segment1 = rand() % 65536;
    int segment2 = rand() % 65536;
    sprintf(dest, "0x%04X-%04X", segment1, segment2);
}

int taskNumber = 0;
int logNumber = 0;
int todoNumber = 0;

static inline int PCS_Boot_Protocol() {
    remove(DATA_PATH "temp.txt");
    printf("\x1B[2J\x1B[H");
    clear_text();

    //task + log + todo numbering check
    FILE *ptRead = fopen(DATA_PATH "pcs-tasks.txt", "r");
    if (ptRead != NULL) {
        char line[1024] = "";
        int tempID;
        while (fgets(line, sizeof(line), ptRead)) {
            if (sscanf(line, "%d|", &tempID) == 1) {
                if (tempID > taskNumber) {
                taskNumber = tempID;
                }
            }
        }
        fclose(ptRead);
    }

    
    FILE *plRead = fopen(DATA_PATH "pcs-logs.txt", "r");
    if (plRead != NULL) {
        char line[1024] = "";
        int tempID;
        while (fgets(line, sizeof(line), plRead)) {
            if (sscanf(line, "%d|", &tempID) == 1) {
                if (tempID > logNumber) {
                    logNumber = tempID;
                }
            }
        }
        fclose(plRead);
    }

    
    FILE *ptoRead = fopen(DATA_PATH "pcs-todo.txt", "r");
    if (ptoRead != NULL) {
        char line[1024] = "";
        int tempID;
        while (fgets(line, sizeof(line), ptoRead)) {
            if (sscanf(line, "%d|", &tempID) == 1) {
                if (tempID > todoNumber) {
                    todoNumber = tempID;
                }
            }
        }
        fclose(ptoRead);
    }

    //theme + user data
    char *filePath = DATA_PATH "pcs-user-data.txt";
    char *tempPath = DATA_PATH "temp.txt";

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

    FILE *f = fopen(filePath, "r");
    if (f) {
        if (fgets(line, sizeof(line), f) != NULL) {
            if (strncmp(line, "0|", 2) == 0 && strstr(line, "0x")) {
                sscanf(line, "0| %s", userHexID);
                hasExistingID = true;
            }
        }
        fclose(f);
    }

    if (!hasExistingID) {
		generate_hex_id(userHexID);

		FILE *pR = fopen(filePath, "r");
		FILE *pT = fopen(tempPath, "w");

		if (pR == NULL || pT == NULL) {
			printf("%s\aCRITICAL ERROR: AccelX Sector Unreachable.\n%s", ERR_C, MAIN_C);
			if (pR) fclose(pR); if (pT) fclose(pT);
		}

		char bufferread[500] = "";
		bool idWritten = false;
		char timestamp[30];
		time_t now = time(NULL);
		strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

		while (fgets(bufferread, sizeof(bufferread), pR) != NULL) {
			if (strncmp(bufferread, "0|", 2) == 0) {
				if (strstr(bufferread, "0x") == NULL) {
					fprintf(pT, "0| %s\n", userHexID);
				} else {
					fprintf(pT, "%s", bufferread);
				}
				idWritten = true;
			} else if (strncmp(bufferread, "7|", 2) == 0) {
				fprintf(pT, "7| %s\n", timestamp);
			} else {
				fprintf(pT, "%s", bufferread);
			}
		}

		if (!idWritten) {
			rewind(pR);
			fclose(pT);
			pT = fopen(tempPath, "w");

            fprintf(pT, "0| %s\n", userHexID);
			fprintf(pT, "1| Operator\n2| Country\n3| MM/DD/YYYY\n4| Gender\n5| Height\n6| Weight\n");
			fprintf(pT, "7| %s\n", timestamp);
			fprintf(pT, "8| 1\n");

			while (fgets(bufferread, sizeof(bufferread), pR) != NULL) {
				int prefix = atoi(bufferread);
				if (prefix == 0 && strncmp(bufferread, "0|", 2) != 0) {
					fprintf(pT, "%s", bufferread);
				} else if (prefix > 8 || prefix < 0) {
					fprintf(pT, "%s", bufferread);
				}
			}
		}

		fclose(pR);
		fclose(pT);

		remove(filePath);
		if (rename(tempPath, filePath) != 0) {
			printf("%s\aCRITICAL FAILURE: Write-Shield active.\n%s", ERR_C, MAIN_C);
		} else {
			hasExistingID = true;
		}
	}

    FILE *u = fopen(filePath, "r");
    if (u == NULL) {
        printf("Error: Could not open file at %s\n", filePath);
    } else {
        while (fgets(linetheme, sizeof(linetheme), u) != NULL) {
            if (strncmp(linetheme, "1|", 2) == 0) {
                sscanf(linetheme, "1| %s", username);
                break;
            }
        }
        fclose(u);
    }

    //Actual Protocol Initiation + Functions
        printf("%s", theme->background_color);

        if (rectified == 0) {
            apply_theme();
            show_cmd_prmt_load();
            wait_seconds(0.5);
            show_loading_long();
            sprintf(welcomeMsg, "Identity Confirmed: User '%s' recognized on the Grid.", userHexID);
            wait_seconds(1.5);
            printf("\n%s\nWelcome %s.\n", welcomeMsg, username);
            wait_seconds(0.5);
            printf("%s\nADVICE: Type 'pcs help' to view available command.%s\n", SUCCESS_C, MAIN_C);
        } else if (rectified == 2) {
            apply_theme();
            xpress_show_cmd_prmt_load();
            wait_seconds(0.5);
            show_loading_short();

            sprintf(welcomeMsg, "Identity Confirmed: User '%s' recognized on the Grid.", userHexID);
            wait_seconds(1);
            printf( "%s"">>> GLOBAL DATA STREAM CLEARED: 0x00h bits returned to the Grid.\n"  "%s", SUCCESS_C, MAIN_C);
            wait_seconds(1);
            printf("\n%s\nWelcome %s.\n", welcomeMsg, username);
            rectified = 0;
        } else if (rectified == 3) {
            apply_theme();
            xpress_show_cmd_prmt_load();
            wait_seconds(0.5);
            show_loading_short();

            sprintf(welcomeMsg, "Identity Confirmed: User '%s' recognized on the Grid.", userHexID);
            wait_seconds(1.5);
            printf("\n%s\nWelcome %s.\n", welcomeMsg, username);
            rectified = 0;
        }

        wait_seconds(1);
}

#endif
