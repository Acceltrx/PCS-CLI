#ifndef PCS_USR_CONFIG_THME_H
#define PCS_USR_CONFIG_THME_H

#include "pcs-themes.h"
#include "pcs-data-types.h"
#include "pcs-ui.h"
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

extern char inputColor[30];
extern char mainColor[30];
extern char accentColor[30];
extern char successColor[30];
extern char errorColor[30];
extern char backgroundColor[30];
extern bool continueFlag;
extern bool breakFlag;
extern int targetthemeID;

static inline void load_configured_theme() {
    char *filePath = DATA_PATH "pcs-themes.txt";
	char *tempPath = DATA_PATH "temp.txt";

	FILE *pR = fopen(filePath, "r");
	FILE *pT = fopen(tempPath, "w");

    fseek(pR, 0, SEEK_END);
    if (ftell(pR) == 0) {
        printf("%s\aTRACE FAILURE: Theme sector not detected in the Grid database.\n", ERR_C);
        continueFlag = true;
        return;
    }
    rewind(pR);

	bool found = false;
	char bufferRead[500] = "";

	if (pR == NULL || pT == NULL) {
		printf("%s\aCRITICAL ERROR: AccelX Sector Unreachable. Unable to establish a handshake with the database.\n%s", ERR_C, MAIN_C);
		if (pR) fclose(pR);
		if (pT) fclose(pT);
	} else {
		while (fgets(bufferRead, sizeof(bufferRead), pR) != NULL) {
			if (!found && strncmp(bufferRead, "0|", 2) == 0) {
				found = true;
				fprintf(pT, "0| 9\n");
			} else {
				fprintf(pT, "%s", bufferRead);
			}
		}

		fclose(pR);
		fclose(pT);

		if (found) {
			remove(filePath);
			if (rename(tempPath, filePath) != 0) {
				printf("%s\aCRITICAL FAILURE: AccelX Write-Shield active. Unable to commit changes.\n%s", ERR_C, MAIN_C);
			} else {
				printf("\n%sPROTOCOL SUCCESS: Environment parameters updated to ID: %d %s", SUCCESS_C, targetthemeID, MAIN_C);
				printf("\nInitiating System Reboot");
				showdots();
				printf("\n%sRebooting...", ACCENT_C);
				wait_seconds(1);
                breakFlag = true;
                return;
			}
		} else {
			remove(tempPath);
			printf("%s\aTRACE FAILURE: Theme sector not detected in the Grid database.\n%s", ERR_C, MAIN_C);
		}
	}
}

static inline void new_configured_theme() {
    char rawHexCodes[5][10] = {0};
    char *prompts[] = {"main", "accent", "success", "error", "background"};
    char *page[] = {"[=----] 1/5", "[==---] 2/5", "[===--] 3/5", "[====-] 4/5", "[=====] 5/5"};

    for (int i = 0; i < 5; i++) {
        int r, g, b;
        bool text = false;
        printf("%s%s >> Enter hex code for %s color:%s ", page[i], ACCENT_C, prompts[i], MAIN_C);
        
        if (fgets(inputColor, sizeof(inputColor), stdin) == NULL) continue;
        inputColor[strcspn(inputColor, "\n")] = '\0';


        if (strcmp(inputColor, "next") == 0) {
            text = true;
            if (i == 4) {
                i = -1;
                continue;
            } else {
                continue;
            }
        } 

        if (strcmp(inputColor, "prev") == 0) {
            text = true;
            if (i > 0) i -= 2;
            else i = 3;
            continue;
        } 

        if (strcmp(inputColor, "cancel") == 0) {
            text = true;
            printf( "%sOPERATION ABORTED: User intervention detected. Task sequence terminated.\n%s", ERR_C, MAIN_C);
            continueFlag = true; 
            return;
        }

        if (strlen(inputColor) == 0) { 
            printf("%s\aTRACE ERROR: Hex marker null.%s\n\n", ERR_C, MAIN_C);
            i--; continue; 
        }

        if (inputColor[0] != '#' || strlen(inputColor) != 7) {
            printf("\a%sSYNTAX ERROR: Invalid format (use #3c30ec)%s\n\n", ERR_C, MAIN_C);
            i--; continue;
        }

        if (sscanf(&inputColor[1], "%02x%02x%02x", &r, &g, &b) != 3) {
            printf("\a%sSYNTAX ERROR: Invalid hex values (0-9, a-f only)%s\n\n", ERR_C, MAIN_C);
            i--; continue;
        }

        if (!text) {
            strcpy(rawHexCodes[i], inputColor);
        }
        
        switch(i) {
            case 0: sprintf(mainColor, "\x1B[38;2;%d;%d;%dm", r, g, b); break;
            case 1: sprintf(accentColor, "\x1B[38;2;%d;%d;%dm", r, g, b); break;
            case 2: sprintf(successColor, "\x1B[38;2;%d;%d;%dm", r, g, b); break;
            case 3: sprintf(errorColor, "\x1B[38;2;%d;%d;%dm", r, g, b); break;
            case 4: sprintf(backgroundColor, "\x1B[48;2;%d;%d;%dm\x1b[2J\x1b[H", r, g, b); break;
        }
    }

    char *filePath = DATA_PATH "pcs-themes.txt";
    char *tempPath = DATA_PATH "temp.txt";
    FILE *pR = fopen(filePath, "r");
    FILE *pT = fopen(tempPath, "w");

    if (pR && pT) {
        char bufferRead[500];

        while (fgets(bufferRead, sizeof(bufferRead), pR)) {
            bool lineReplaced = false;

            for (int j = 0; j < 5; j++) {
                char key[10];
                sprintf(key, "hex%d|", j + 1);

                if (strstr(bufferRead, key) == bufferRead) {
                    if (strlen(rawHexCodes[j]) > 0) {
                        fprintf(pT, "%s %s\n", key, rawHexCodes[j]);
                        lineReplaced = true;
                    } 
                    break;
                }
            }

            if (!lineReplaced && strncmp(bufferRead, "0|", 2) == 0) {
                fprintf(pT, "0| 9\n");
                lineReplaced = true;
            }

            if (!lineReplaced) {
                fprintf(pT, "%s", bufferRead);
            }
        }

        fclose(pR);
        fclose(pT);
        remove(filePath);
        rename(tempPath, filePath);

        printf("\n%sPROTOCOL SUCCESS: Environment parameters updated to ID: 9 %s", SUCCESS_C, MAIN_C);
        printf("\nInitiating System Reboot");
        showdots();
        printf("\n%sRebooting...", ACCENT_C);
        wait_seconds(1);

        breakFlag = true; 
        return;
    } else {
        if(pR) fclose(pR);
        if(pT) fclose(pT);
        printf("%s\aCRITICAL ERROR: AccelX Sector Unreachable. Unable to establish a handshake with the database.\n%s", ERR_C, MAIN_C);
    }
}

#endif