#include "pcs-colors.h"
#include "pcs-ui.h"
#include "pcs-themes.h"
#include "pcs-data-types.h"
#include "pcs-boot-prtcl.h"
#include "pcs-version.h"
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

static inline int PCS_Protocol() {
        PCS_Boot_Protocol();

        while (2) {
            printf("\n%sACCELX://SYSTEM/SECTOR_01>%s", ACCENT_C, MAIN_C);
            fgets(text, sizeof(text), stdin);
            text[strcspn(text, "\n")] = '\0';

            if(strstr(text, addtaskID) != NULL) { //add task +++
                char *start = strchr(text, '\"');
                char *end = strrchr(text, '\"');
                bool dataAnchor = false;
                FILE *ptR = fopen(DATA_PATH "pcs-tasks.txt", "r");
                
                if (ptR != NULL) {
                    int firstChar = fgetc(ptR);
                    if (firstChar != EOF) {
                        dataAnchor = true;
                    }
                    fclose(ptR); // Close it immediately after checking
                }

                if (dataAnchor) {
                    if (start != NULL && end != NULL && start != end) { 
                        FILE *pF = fopen(DATA_PATH "pcs-tasks.txt", "a");
                        if (pF != NULL) {
                            char taskName[100]; 
                            int length = end - start - 1;

                            if (length > 50) {
                                printf("%s\aSYNTAX ERROR: Objective too large. Data must be within 50 characters.\n%s", ERR_C, MAIN_C);
                                continue; 
                            }

                            strncpy(taskName, start + 1, length);
                            taskName[length] = '\0';

                            taskNumber += 1;
                            fprintf(pF, "%d| %s | [!] |\n", taskNumber, taskName);
                            printf( "%s" "PROTOCOL SUCCESS: New objective \"%s\" compiled into the Grid.\n"  "%s", SUCCESS_C, taskName, MAIN_C);
                            fclose(pF);
                        } else {
                            printf("%s\aCRITICAL ERROR: Write access to Grid denied.%s\n", ERR_C, MAIN_C);
                        }
                    } else {
                        printf( "%s" "\aSYNTAX ERROR: Objective string missing. Encapsulate data in quotes for Grid compilation. (Example: pcs add task \"objective\")\n"  "%s", ERR_C, MAIN_C);
                    }
                } else {
                    printf("%s\aERROR: Data stream returned 'NULL'.\n", ERR_C);
                    printf("%sADVICE: Execute 'pcs new task list' to establish a data-anchor.%s\n", SUCCESS_C, MAIN_C);
                }
            } else if (strstr(text, newtasklistID) != NULL) { //new task list 
                char tasklistdate[100] = "";
                printf(">> Input Temporal Coordinates for the new Cycle (MM/DD/YYYY): ");
                fgets(tasklistdate, sizeof(tasklistdate), stdin);
                tasklistdate[strcspn(tasklistdate, "\n")] = '\0';

                int reset = 2;
                int m, d, y;
                        int matches = sscanf(tasklistdate, "%d/%d/%d", &m, &d, &y);

                        if (matches == 3) {
                            if (m >= 1 && m <= 12 && d >= 1 && d <= 31 && y >= 0 && y <= 9999) {
                                reset = 1;
                            } else {
                                printf( "%s" "\aSYNTAX ERROR: Date values out of bounds (1-12/1-31/00-9999).\n"  "%s", ERR_C, MAIN_C);
                                reset = 2;
                            }
                        }
            
                if (strcmp(tasklistdate, "cancel") == 0) {
                    printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                } else if (strlen(tasklistdate) == 0) {
                    printf( "%s" "\aTRACE ERROR: Temporal marker null. Re-input MM/DD/YYYY to synchronize local clock.\n"  "%s", ERR_C, MAIN_C);
                } else if (reset == 1) {
                    
                    FILE *pR = fopen(DATA_PATH "pcs-tasks.txt", "r");
                    FILE *pA = fopen(DATA_PATH "pcs-archive-tasks.txt", "a");
                    char bufferread[500];
                    bool startPrinting = false;

                    while (fgets(bufferread, sizeof(bufferread), pR) != NULL) {
                        if (!startPrinting) {
                            startPrinting = true; 
                        }

                        if (startPrinting) {
                            fprintf(pA, "%s", bufferread);
                        }
                    }
                    fprintf(pA, "\n");
                    fclose(pR);
                    fclose(pA);

                    FILE *pW = fopen(DATA_PATH "pcs-tasks.txt", "w");
                    taskNumber = 0;
                    fprintf(pW, "0| %02d/%02d/%04d\n", m, d, y);
                    printf( "%s" "PROTOCOL SUCCESS: Grid re-initialized. Fresh cycle started.\n"  "%s", SUCCESS_C, MAIN_C);
                    fclose(pW);
                } else if (reset == 2) {
                    printf( "%s" "\aSYNTAX ERROR: Temporal marker invalid. Re-input MM/DD/YYYY to synchronize local clock.\n"  "%s", ERR_C, MAIN_C);
                }
            } else if (strstr(text, seetasklistID) != NULL) { //see task list 
                FILE *ptR = fopen(DATA_PATH "pcs-tasks.txt", "r");
            
                if (ptR == NULL) {
                    printf("%s\aCRITICAL ERROR: Sector file missing.%s\n", ERR_C, MAIN_C);
                    continue;
                }

                char buffer1read[500];
                char complete[10] = "[+]";
                char pending[10] = "[!]";
                char readcomplete[500];

            // Check if the file is empty by trying to read the first character
                int firstChar = fgetc(ptR);
                if (firstChar == EOF) {
                    printf("%s\aERROR: Data stream returned 'NULL'.\n", ERR_C);
                    printf("%sADVICE: Execute 'pcs new task list' to establish a data-anchor.%s\n", SUCCESS_C, MAIN_C);
                } else {
                    ungetc(firstChar, ptR); // Put the character back so the loop doesn't miss it
                
                    printf("\n%s=====[-ACTIVE-GRID-OBJECTIVES-]=====%s\n", ACCENT_C, MAIN_C);
                
                    while (fgets(buffer1read, sizeof(buffer1read), ptR) != NULL) {
                        if (strstr(buffer1read, complete) != NULL) {
                            printf("%s%s%s", SUCCESS_C, buffer1read, MAIN_C);
                        } else if (strstr(buffer1read, pending) != NULL) {
                            printf("%s%s%s", ERR_C, buffer1read, MAIN_C);
                        } else {
                            printf("%s", buffer1read);
                        }
                    }
                }
                fclose(ptR);
            } else if (strstr(text, completetaskID) != NULL) { //task completion 
                char *filePath = DATA_PATH "pcs-tasks.txt";
                char *tempPath = DATA_PATH "temp.txt";

                FILE *pR = fopen(filePath, "r");
                FILE *pT = fopen(tempPath, "w");

                bool found = false;
                char bufferread[500] = "";
                bool dataAnchor = false;

                if (pR == NULL || pT == NULL) {
                    printf( "%s" "\aCRITICAL ERROR: Memory Sector Unreachable. Unable to establish a link to the task database.\n"  "%s", ERR_C, MAIN_C);
                    if (pR) fclose(pR);
                    if (pT) fclose(pT);
                    continue; 
                }

                int firstChar = fgetc(pR);
                if (firstChar != EOF) {
                    dataAnchor = true;
                    ungetc(firstChar, pR); 
                }

                if (dataAnchor) {
                    char targetIDStr[10] = "";
                    printf(">> Specify Target ID for Resolution Protocol: ");
                    fgets(targetIDStr, sizeof(targetIDStr), stdin);
                    targetIDStr[strcspn(targetIDStr, "\n")] = '\0';

                    char searchID[20];
                    sprintf(searchID, "%s|", targetIDStr);
    
                    if (strlen(targetIDStr) == 0) {
                        printf( "%s" "\aTRACE ERROR: Target ID null. Re-input valid identifier for Grid processing.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else if (strstr(targetIDStr, "cancel") != NULL) {
                        printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else {
                        int i;
                        char check;
                        int matches1 = sscanf(targetIDStr, "%d %c", &i, &check);

                        if (matches1 == 1) {
                            if (i >= 1 && i <= 99999999) {
                                while (fgets(bufferread, sizeof(bufferread), pR) != NULL) { 
                                    if (!found && strstr(bufferread, searchID) == bufferread && strcmp(targetIDStr, "0") != 0) {
                                        found = true;
                                        char *ptr = strstr(bufferread, "[!]");
                    
                                        if (ptr != NULL) {
                                            *(ptr + 1) = '+';
                                            printf( "%s" "PROTOCOL COMPLETE: Task #%s neutralized. Cycle synchronized.\n"  "%s", SUCCESS_C, targetIDStr, MAIN_C);
                                        } else {
                                            printf( "%s" "\aREDUNDANCY ALERT: Task #%s is already offline. Target state is already resolved.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                        }
                                    } 
                                    fprintf(pT, "%s", bufferread);
                                } 
        
                                fclose(pR);
                                fclose(pT);

                                if (found) {
                                    remove(filePath);
                                    if (rename(DATA_PATH "temp.txt", DATA_PATH "pcs-tasks.txt") != 0) {
                                        printf( "%s" "\aCRITICAL Failure: Bit-stream corruption detected. Unable to write changes to Sector Data.", ERR_C);
                                    }
                                } else {
                                    remove(tempPath);
                                    if (strcmp(targetIDStr, "0") == 0) {
                                        printf( "%s" "\aACCESS DENIED: ID 0 is protected by AccelX Systems Protocol. Select an unassigned user-sector ID.\n"  "%s", ERR_C, MAIN_C);
                                    } else {
                                        printf( "%s" "\aSEARCH FAILURE: ID #%s not detected. Target has been derezzed or never initialized.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                    }
                                }
                            }
                        } else {
                            printf( "%s" "\aSYNTAX ERROR: Invalid Bit-Stream. Data must be an integer value"  "%s\n", ERR_C, MAIN_C);
                            fclose(pR);
                            fclose(pT);
                            remove(tempPath);
                        }
                    }   
                } else {
                    printf("%s\aERROR: Data stream returned 'NULL'.\n", ERR_C);
                    printf("%sADVICE: Execute 'pcs new task list' to establish a data-anchor.%s\n", SUCCESS_C, MAIN_C);
                    fclose(pR);
                    fclose(pT);
                    remove(tempPath);
                }
            } else if (strstr(text, resettaskID) != NULL) { //task reset 
                char *filePath = DATA_PATH "pcs-tasks.txt";
                char *tempPath = DATA_PATH "temp.txt";

                FILE *pR = fopen(filePath, "r");
                FILE *pT = fopen(tempPath, "w");

                bool found = false;
                char bufferread[500] = "";
                bool dataAnchor = false;

                if (pR == NULL || pT == NULL) {
                    printf( "%s" "\aCRITICAL ERROR: AccelX Sector Unreachable. Unable to establish a handshake with the task database.\n"  "%s", ERR_C, MAIN_C);
                    if (pR) fclose(pR);
                    if (pT) fclose(pT);
                    continue; 
                }

                int firstChar = fgetc(pR);
                if (firstChar != EOF) {
                    dataAnchor = true;
                    ungetc(firstChar, pR); 
                }

                if (dataAnchor) {
                    char targetIDStr[10] = "";
                    printf(">> Identify Target ID for Re-initialization Protocol: ");
                    fgets(targetIDStr, sizeof(targetIDStr), stdin);
                    targetIDStr[strcspn(targetIDStr, "\n")] = '\0';

                    char searchID[20];
                    sprintf(searchID, "%s|", targetIDStr);

                    if (strlen(targetIDStr) == 0) {
                        printf( "%s" "\aTRACE ERROR: Target ID null. Re-input valid identifier for Grid processing.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else if (strstr(targetIDStr, "cancel") != NULL) {
                        printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else {
                        int i;
                        char check;
                        int matches1 = sscanf(targetIDStr, "%d %c", &i, &check);

                        if (matches1 == 1) {
                            if (i >= 1 && i <= 99999999) {
                                while (fgets(bufferread, sizeof(bufferread), pR) != NULL) { 
                                    if (!found && strstr(bufferread, searchID) == bufferread && strcmp(targetIDStr, "0") != 0) {
                                        found = true;
                                        char *ptr = strstr(bufferread, "[+]");

                                        if (ptr != NULL) {
                                            *(ptr + 1) = '!';
                                            printf( "%s" "\aGRID ALERT: Task #%s re-energized. Entry has been pushed back to the Active Buffer.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                        } else {
                                            printf( "%s" "\aLOGIC ERROR: Task #%s bit-state mismatch. Sector is either already active or pending initialization.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                        }
                                    } 
                                    fprintf(pT, "%s", bufferread);
                                } 
                                fclose(pR);
                                fclose(pT);
                                if (found) {
                                    remove(filePath);
                                    if (rename(tempPath, filePath) != 0) {
                                        printf( "%s" "\aCRITICAL FAILURE: AccelX Write-Shield active. Unable to commit changes", ERR_C);
                                    }
                                } else {
                                    remove(tempPath);
                                    if (strcmp(targetIDStr, "0") == 0) {
                                        printf( "%s" "\aACCESS DENIED: ID 0 is protected by AccelX Systems Protocol. Select an unassigned user-sector ID\n\n"  "%s", ERR_C, MAIN_C);
                                    } else {
                                        printf( "%s" "\aTRACE FAILURE: ID #%s not detected on the Grid. Entity may have been Derezzed.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                    }
                                }
                            }
                        } else {
                            printf( "%s" "\aSYNTAX ERROR: Invalid Bit-Stream. Data must be an integer value"  "%s\n", ERR_C, MAIN_C);
                            fclose(pR);
                            fclose(pT);
                            remove(tempPath);
                        }
                    }
                } else {
                    printf("%s\aERROR: Data stream returned 'NULL'.\n", ERR_C);
                    printf("%sADVICE: Execute 'pcs new task list' to establish a data-anchor.%s\n", SUCCESS_C, MAIN_C);
                    fclose(pR);
                    remove(tempPath);
                }
            } else if (strstr(text, deletetaskID) != NULL) { //task delete 
                char *filePath = DATA_PATH "pcs-tasks.txt";
                char *tempPath = DATA_PATH "temp.txt";

                FILE *pR = fopen(filePath, "r");
                FILE *pT = fopen(tempPath, "w");

                bool found = false;
                char bufferread[500] = "";
                bool dataAnchor = false;

                if (pR == NULL || pT == NULL) {
                    printf( "%s" "\aCRITICAL ERROR: AccelX Sector Unreachable. Unable to establish a handshake with the task database.\n"  "%s", ERR_C, MAIN_C);
                    if (pR) fclose(pR);
                    if (pT) fclose(pT);
                    continue; 
                }

                if (fgets(bufferread, sizeof(bufferread), pR) != NULL) {
                    if (strncmp(bufferread, "0|", 2) != 0) {
                        dataAnchor = true; 
                    } 
                }

                if (dataAnchor) {
                    char targetIDStr[10] = "";
                    printf(">> Identify Target ID for Data Derezz Protocol: ");
                    fgets(targetIDStr, sizeof(targetIDStr), stdin);
                    targetIDStr[strcspn(targetIDStr, "\n")] = '\0';

                    char searchID[20];
                    sprintf(searchID, "%s|", targetIDStr);

                    if (strlen(targetIDStr) == 0) {
                        printf( "%s" "\aTRACE ERROR: Target ID null. Re-input valid identifier for Grid processing.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else if (strstr(targetIDStr, "cancel") != NULL) {
                        printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else {
                        int i;
                        char check;
                        int matches1 = sscanf(targetIDStr, "%d %c", &i, &check);

                        if (matches1 == 1) {
                            if (i >= 1 && i <= 99999999) {
                                while (fgets(bufferread, sizeof(bufferread), pR) != NULL) { 
                            if (!found && strstr(bufferread, searchID) == bufferread && strcmp(targetIDStr, "0") != 0) {
                                found = true;

                                if (searchID != NULL) {
                                    sprintf(bufferread, "\0", bufferread);  
                                    printf( "%s" "PROTOCOL SUCCESS: Task #%s has successfully been derezzed.\n"  "%s", SUCCESS_C, targetIDStr, MAIN_C);
                                }
                            }
                            fprintf(pT, "%s", bufferread);
                        }
                        fclose(pR);
                        fclose(pT);
                        if (found) {
                            remove(filePath);
                            if (rename(tempPath, filePath) != 0) {
                                printf( "%s" "\aCRITICAL FAILURE: AccelX Write-Shield active. Unable to commit changes", ERR_C);
                            }
                        } else {
                            remove(tempPath);
                            if (strcmp(targetIDStr, "0") == 0) {
                                printf( "%s" "\aACCESS DENIED: ID 0 is protected by AccelX Systems Protocol. Select an unassigned user-sector ID\n\n"  "%s", ERR_C, MAIN_C);
                            } else {
                                printf( "%s" "\aTRACE FAILURE: ID #%s not detected on the Grid. Entity may have been Derezzed.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                            }
                        }
                    }
                            } else {
                                printf( "%s" "\aSYNTAX ERROR: Invalid Bit-Stream. Data must be an integer value"  "%s\n", ERR_C, MAIN_C);
                            }
                        }
                        
                        
                } else {
                    printf("%s\aERROR: Data stream returned 'NULL'.\n", ERR_C);
                    printf("%sADVICE: Execute 'pcs new task list' to establish a data-anchor.%s\n", SUCCESS_C, MAIN_C);
                    fclose(pR);
                    remove(tempPath);
                }
            } else if (strstr(text, newlogID) != NULL) { //new log 
                char newlogdate[100] = "";
                printf(">>Input Temporal Coordinates for New Cycle (MM/DD/YYYY): ");
                fgets(newlogdate, sizeof(newlogdate), stdin);
                newlogdate[strcspn(newlogdate, "\n")] = '\0';

                int reset = 2;
                int m, d, y;
                        int matches = sscanf(newlogdate, "%d/%d/%d", &m, &d, &y);

                        if (matches == 3) {
                            if (m >= 1 && m <= 12 && d >= 1 && d <= 31 && y >= 0 && y <= 9999) {
                                reset = 1;
                            } else {
                                printf( "%s" "\aSYNTAX ERROR: Date values out of bounds (1-12/1-31/00-9999).\n"  "%s", ERR_C, MAIN_C);
                                reset = 2;
                            }
                        }
            
                if (strcmp(newlogdate, "cancel") == 0) {
                    printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                } else if (strlen(newlogdate) == 0) {
                    printf( "%s" "\aINPUT FAILURE: Temporal marker null. Re-input MM/DD/YYYY to synchronize local clock.\n"  "%s", ERR_C, MAIN_C);
                } else if (reset == 1) {
                    FILE *pR = fopen(DATA_PATH "pcs-logs.txt", "r");
                    FILE *pA = fopen(DATA_PATH "pcs-archive-logs.txt", "a");
                    char bufferread[500];
                    bool startPrinting = false;

                    while (fgets(bufferread, sizeof(bufferread), pR) != NULL) {
                        if (!startPrinting) {
                            startPrinting = true; 
                        }

                        if (startPrinting) {
                            fprintf(pA, "%s", bufferread);
                        }
                    }
                    fprintf(pA, "\n");
                    fclose(pR);
                    fclose(pA);

                    FILE *pW = fopen(DATA_PATH "pcs-logs.txt", "w");
                    taskNumber = 0;
                    fprintf(pW, "0| %02d/%02d/%04d\n", m, d, y);
                    printf( "%s" "PROTOCOL SUCCESS: New AccelX Memory Stream is now online.\n"  "%s", SUCCESS_C, MAIN_C);
                    fclose(pW);
                } else if (reset == 2) {
                    printf( "%s" "\aSYNTAX ERROR: Temporal marker invalid. Re-input MM/DD/YYYY to synchronize local clock.\n"  "%s", ERR_C, MAIN_C);
                }
            } else if (strstr(text, addlogID) != NULL) { //add log +++
                FILE *pFa = fopen(DATA_PATH "pcs-logs.txt", "a");
                FILE *pFr = fopen(DATA_PATH "pcs-logs.txt", "r");
                char logcontent[150]; 
                bool dataAnchor = false;
                    
                if (pFr != NULL) {
                    int firstChar = fgetc(pFr);
                    if (firstChar != EOF) {
                        dataAnchor = true;
                    }
                    fclose(pFr);
                }

                if (dataAnchor) {
                    printf(">> Input Thought-Stream for local buffer: ");
                    fgets(logcontent, sizeof(logcontent), stdin);
                    logcontent[strcspn(logcontent, "\n")] = '\0';

                    time_t rawtime;
                    struct tm *timeinfo;
                    char timeStr[10];
                    time(&rawtime);
                    timeinfo = localtime(&rawtime);
                    strftime(timeStr, sizeof(timeStr), "%H:%M", timeinfo);

                    if (strlen(logcontent) == 0) {
                        printf( "%s" "\aERROR: No data detected for commit. Re-initialize packet stream.\n"  "%s", ERR_C, MAIN_C);
                        if (pFa) fclose(pFa);
                    } else if (strcmp(logcontent, "cancel") == 0) {
                        printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                        if (pFa) fclose(pFa);
                    } else {
                        if (strlen(logcontent) > 100) {
                            printf("%s\aSYNTAX ERROR: Objective too large. Data must be within 100 characters.\n%s", ERR_C, MAIN_C);
                            continue; 
                        }
                        
                        if (pFa != NULL) {
                            logNumber += 1;
                            fprintf(pFa, "%d| %s | %s | {~} |\n", logNumber, timeStr, logcontent);
                            printf( "%s" "PROTOCOL SUCCESS: Memory packet integrated into AccelX Archive.\n"  "%s", SUCCESS_C, MAIN_C);
                            fclose(pFa);
                        }
                    }
                } else {
                    printf("%s\aERROR: Data stream returned 'NULL'.\n", ERR_C);
                    printf("%sADVICE: Execute 'pcs new log' to establish a data-anchor.%s\n", SUCCESS_C, MAIN_C);
                    if (pFa) fclose(pFa);
                }
            } else if (strstr(text, deletelogID) != NULL) { //delete log 
                char *filePath = DATA_PATH "pcs-logs.txt";
                char *tempPath = DATA_PATH "temp.txt";

                FILE *pR = fopen(filePath, "r");
                FILE *pT = fopen(tempPath, "w");

                bool found = false;
                char bufferread[500] = "";
                bool dataAnchor = false;

                if (pR == NULL || pT == NULL) {
                    printf( "%s" "\aCRITICAL ERROR: AccelX Sector Unreachable. Unable to establish a handshake with the task database.\n"  "%s", ERR_C, MAIN_C);
                    if (pR) fclose(pR);
                    if (pT) fclose(pT);
                    continue; 
                }

                int firstChar = fgetc(pR);
                if (firstChar != EOF) {
                    dataAnchor = true;
                    ungetc(firstChar, pR); 
                }

                if (dataAnchor) {
                    char targetIDStr[10] = "";
                    printf(">>Identify Target ID for Data Derezz Protocol: ");
                    fgets(targetIDStr, sizeof(targetIDStr), stdin);
                    targetIDStr[strcspn(targetIDStr, "\n")] = '\0';

                    char searchID[20];
                    sprintf(searchID, "%s|", targetIDStr);

                    if (strlen(targetIDStr) == 0) {
                        printf( "%s" "\aTRACE ERROR: Target ID null. Re-input valid identifier for Grid processing.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else if (strstr(targetIDStr, "cancel") != NULL) {
                        printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else {
                        int i;
                        char check;
                        int matches1 = sscanf(targetIDStr, "%d %c", &i, &check);

                        if (matches1 == 1) {
                            if (i >= 1 && i <= 99999999) {
                                while (fgets(bufferread, sizeof(bufferread), pR) != NULL) { 
                                    if (!found && strstr(bufferread, searchID) == bufferread && strcmp(targetIDStr, "0") != 0) {
                                        found = true;
                                        printf( "%s" "PROTOCOL SUCCESS: Data string #%s has successfully been derezzed.\n"  "%s", SUCCESS_C, targetIDStr, MAIN_C);
                                        continue; // Skip writing this line to pT
                                    }
                                    fprintf(pT, "%s", bufferread);
                                }
                                fclose(pR);
                                fclose(pT);
                                if (found) {
                                    remove(filePath);
                                    rename(tempPath, filePath);
                                } else {
                                    remove(tempPath);
                                    if (strcmp(targetIDStr, "0") == 0) {
                                        printf( "%s" "\aACCESS DENIED: ID 0 is protected by AccelX Systems Protocol.\n"  "%s", ERR_C, MAIN_C);
                                    } else {
                                        printf( "%s" "\aTRACE FAILURE: ID #%s not detected on the Grid.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                    }
                                }
                            }
                        } else {
                            printf( "%s" "\aSYNTAX ERROR: Invalid Bit-Stream. Data must be an integer value"  "%s\n", ERR_C, MAIN_C);
                            fclose(pR);
                            fclose(pT);
                            remove(tempPath);
                        }
                    }
                } else {
                    printf("%s\aERROR: Data stream returned 'NULL'.\n", ERR_C);
                    printf("%sADVICE: Execute 'pcs new log' to establish a data-anchor.%s\n", SUCCESS_C, MAIN_C);
                    fclose(pR);
                    fclose(pT);
                    remove(tempPath);
                }
            } else if (strstr(text, seelogID) != NULL) { //see log 
                FILE *plR = fopen(DATA_PATH "pcs-logs.txt", "r");
                char bufferread[500];
                bool startPrinting = false;

                if (plR != NULL) {
                    int firstChar = fgetc(plR);
                    if (firstChar != EOF) {
                        startPrinting = true;
                        ungetc(firstChar, plR); 
                    }

                    if (startPrinting) {
                        printf("\n"  "%s"  "=====[-ACTIVE-GRID-LOG-]====="  "%s\n", ACCENT_C, MAIN_C);
                        while (fgets(bufferread, sizeof(bufferread), plR) != NULL) {
                            printf("%s", bufferread);
                        }
                    } else {
                        printf("%s\aERROR: Data stream returned 'NULL'.\n", ERR_C);
                        printf("%sADVICE: Execute 'pcs new log' to establish a data-anchor.%s\n", SUCCESS_C, MAIN_C);
                    }
                    fclose(plR);
                }
            } else if (strstr(text, retrievetaskID) != NULL) { //retrieve task from archive 
                char targettaskdate[100] = "";
                printf(" >>Input Temporal Coordinates for memory packet retrieval (MM/DD/YYYY): ");
                fgets(targettaskdate, sizeof(targettaskdate), stdin);
                targettaskdate[strcspn(targettaskdate, "\n")] = '\0';

                int reset = 2;
                int m, d, y;
                        int matches = sscanf(targettaskdate, "%d/%d/%d", &m, &d, &y);

                        if (matches == 3) {
                            if (m >= 1 && m <= 12 && d >= 1 && d <= 31 && y >= 0 && y <= 9999) {
                                reset = 1;
                                sprintf(targettaskdate, "%02d/%02d/%04d", m, d, y);
                            } else {
                                printf( "%s" "\aSYNTAX ERROR: Date values out of bounds (1-12/1-31/00-9999).\n"  "%s", ERR_C, MAIN_C);
                                continue;
                            }
                        }

                if (strcmp(targettaskdate, "cancel") == 0) {
                    printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                } else if (strlen(targettaskdate) == 0) {
                    printf( "%s" "\aINPUT FAILURE: Temporal marker null. Re-input MM/DD/YYYY for memory packet retrieval.\n"  "%s", ERR_C, MAIN_C);
                } else if (reset == 1) {
                    FILE *pA = fopen(DATA_PATH "pcs-archive-tasks.txt", "r");
        
                    if (pA == NULL) {
                        printf( "%s" "\aCRITICAL ERROR: AccelX Sector Unreachable. Unable to establish a handshake with the task database.\n"  "%s", ERR_C, MAIN_C);
                    } else {
                        char bufferread[500];
                        bool foundDate = false;
                        bool packetFinished = false;
                        char complete[10] = "[+]";
                        char pending[10] = "[!]";
                        char readcomplete[500];

                        while (fgets(bufferread, sizeof(bufferread), pA) != NULL) {
                            if (!foundDate) {
                                if (strstr(bufferread, targettaskdate) != NULL) {
                                    foundDate = true;
                                    printf("\n"  "%s"  "=====[-ACCELX-ARCHIVE-STREAM-]====="  "%s" "\n", ACCENT_C, MAIN_C);
                                    printf( "%s" "%s", MAIN_C, bufferread);
                                }
                            } else {
                                if (bufferread[0] == '\n' || bufferread[0] == '\r' || bufferread[0] == ' ') {
                                    packetFinished = true;
                                    break; 
                                }
                                if (strstr(bufferread, complete) != NULL) {
                                    sprintf(readcomplete,  "%s" "%s" "%s", SUCCESS_C, bufferread, MAIN_C);
                                    printf("%s", readcomplete);
                                } else if (strstr(bufferread, pending) != NULL) {
                                    sprintf(readcomplete,  "%s" "%s" "%s", ERR_C, bufferread, MAIN_C);
                                    printf("%s", readcomplete);
                                } else {
                                    printf("%s", readcomplete);
                                }
                            }
                        }

                        if (!foundDate) {
                            printf( "%s" "\aERROR: No memory packets found for coordinates: %s\n"  "%s", ERR_C, targettaskdate, MAIN_C);
                        } else if (packetFinished || feof(pA)) {
                            printf( "%s"  "======[-END-OF-PACKET-STREAM-]=====\n"  "%s", ACCENT_C, MAIN_C);
                        }
                    fclose(pA);
                    }
                } else if (reset == 2) {
                    printf( "%s" "\aSYNTAX ERROR: Temporal marker invalid. Re-input MM/DD/YYYY to synchronize local clock.\n"  "%s", ERR_C, MAIN_C);
                }
            } else if (strstr(text, retrievelogID) != NULL) { //retrieve log from archive 
                char targetlogdate[100] = "";
                printf(">> Input Temporal Coordinates for memory packet retrieval (MM/DD/YYYY): ");
                fgets(targetlogdate, sizeof(targetlogdate), stdin);
                targetlogdate[strcspn(targetlogdate, "\n")] = '\0';

                int reset = 2;
                int m, d, y;
                        int matches = sscanf(targetlogdate, "%d/%d/%d", &m, &d, &y);

                        if (matches == 3) {
                            if (m >= 1 && m <= 12 && d >= 1 && d <= 31 && y >= 0 && y <= 9999) {
                                reset = 1;
                                sprintf(targetlogdate, "%02d/%02d/%04d", m, d, y);
                            } else {
                                printf( "%s" "\aSYNTAX ERROR: Date values out of bounds (1-12/1-31/00-9999).\n"  "%s", ERR_C, MAIN_C);
                                continue;
                            }
                        }

                if (strcmp(targetlogdate, "cancel") == 0) {
                    printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                } else if (strlen(targetlogdate) == 0) {
                    printf( "%s" "\aINPUT FAILURE: Temporal marker null. Re-input MM/DD/YYYY for memory packet retrieval.\n"  "%s", ERR_C, MAIN_C);
                } else if (reset == 1) {
                    FILE *pA = fopen(DATA_PATH "pcs-archive-logs.txt", "r");
        
                    if (pA == NULL) {
                        printf( "%s" "\aCRITICAL ERROR: AccelX Sector Unreachable. Unable to establish a handshake with the task database.\n"  "%s", ERR_C, MAIN_C);
                    } else {
                        char bufferread[500];
                        bool foundDate = false;
                        bool packetFinished = false;

                        while (fgets(bufferread, sizeof(bufferread), pA) != NULL) {
                            if (!foundDate) {
                                if (strstr(bufferread, targetlogdate) != NULL) {
                                    foundDate = true;
                                    printf("\n"  "%s"  "=====[-ACCELX-ARCHIVE-STREAM-]====="  "%s" "\n", ACCENT_C, MAIN_C);
                                    printf( "%s" "%s", MAIN_C, bufferread);
                                }
                            } else {
                                if (bufferread[0] == '\n' || bufferread[0] == '\r' || bufferread[0] == ' ') {
                                    packetFinished = true;
                                    break; 
                                } else {
                                    printf("%s", bufferread);
                                }
                            }
                        }

                        if (!foundDate) {
                            printf( "%s" "\aERROR: No memory packets found for coordinates: %s\n"  "%s", ERR_C, targetlogdate, MAIN_C);
                        } else if (packetFinished || feof(pA)) {
                            printf( "%s"  "======[-END-OF-PACKET-STREAM-]=====\n"  "%s", ACCENT_C, MAIN_C);
                        }
                    fclose(pA);
                    }
                } else if (reset == 2) {
                    printf( "%s" "\aSYNTAX ERROR: Temporal marker invalid. Re-input MM/DD/YYYY to synchronize local clock.\n"  "%s", ERR_C, MAIN_C);
                }
            } else if (strstr(text, addtodoID) != NULL) { //add todo +++
                char *starttask = strchr(text, '\"');
                char *endtask = strrchr(text, '\"');
                char *startprio = strchr(text, '[');
                char *endprio = strchr(text, ']');
                char *startcat = strchr(text, '(');
                char *endcat = strrchr(text, ')');

                if (starttask != NULL && endtask != NULL && starttask != endtask) {
                    FILE *pA = fopen(DATA_PATH "pcs-todo.txt", "a");
                    char taskName[150]; 
                    char prioType[20]; 
                    char catType[20]; 
                    int tasklength = endtask - starttask - 1;

                    if (tasklength > 100) {
                        printf("%s\aSYNTAX ERROR: Objective too large. Data must be within 100 characters.\n%s", ERR_C, MAIN_C);
                        continue; 
                    }

                    strncpy(taskName, starttask + 1, tasklength);
                    taskName[tasklength] = '\0';
                    
                    if(startprio != NULL && endprio != NULL && startprio != endprio) {
                        int priolength = endprio - startprio - 1;
                        strncpy(prioType, startprio + 1, priolength);
                        if (strstr(prioType, tpLow) != NULL || strstr(prioType, tpMedium) != NULL || strstr(prioType, tpHigh) != NULL || strstr(prioType, tpUrgent) != NULL) {
                            prioType[priolength] = '\0';
                            to_uppercase(prioType);
                        } else {
                            prioType[priolength] = '\0';
                            printf( "%s" "\aERROR: '%s' is not recognized as a persistent task priority.\n"  "%s", ERR_C, prioType, MAIN_C);
                            printf( "%s" "Advise: Use 'urgent', 'high', 'medium', or 'low'.\n"  "%s", SUCCESS_C, MAIN_C);
                            continue;
                        }
                    } else if(startprio == NULL || endprio == NULL) {
                        int NULLpriolength = sizeof(tpMedium);
                        strncpy(prioType, tpMedium, NULLpriolength);
                        prioType[NULLpriolength] = '\0';
                        to_uppercase(prioType);
                    }

                    if(startcat != NULL && endcat != NULL && startcat != endcat) {
                        int catlength = endcat - startcat - 1;
                        strncpy(catType, startcat + 1, catlength);
                        if (strstr(catType, tcPersonal) != NULL || strstr(catType, tcWork) != NULL || strstr(catType, tcFitness) != NULL || strstr(catType, tcSchool) != NULL) {
                            catType[catlength] = '\0';
                            to_uppercase(catType);
                        } else {
                            catType[catlength] = '\0';
                            printf( "%s" "\aERROR: '%s' is not recognized as a persistent task category.\n"  "%s", ERR_C, catType, MAIN_C);
                            printf( "%s" "Advise: Use 'personal', 'work', 'school', or 'fitness'.\n"  "%s", SUCCESS_C, MAIN_C);
                            continue;
                        }
                    } else if(startcat == NULL || endcat == NULL) {
                        int NULLcatlength = sizeof(tcPersonal);
                        strncpy(catType, tcPersonal, NULLcatlength);
                        catType[NULLcatlength] = '\0';
                        to_uppercase(catType);
                    }

                    todoNumber+=1;
                    fprintf(pA, "%d| %s | [!] | %s | %s |\n", todoNumber, taskName, prioType, catType);
                    printf( "%s" "PROTOCOL COMPLETE: New persistent objective \"%s\" compiled into the Grid.\n"  "%s", SUCCESS_C, taskName, MAIN_C);
                    fclose(pA);
                } else {
                    printf( "%s" "\aSYNTAX ERROR: Objective string missing. Encapsulate data in quotes for Grid compilation. (Example: pcs add todo \"objective\" [category] (priority))\n"  "%s", ERR_C, MAIN_C);
                }
            } else if (strstr(text, seetodoID) != NULL) { //see todo 
                FILE *ptR = fopen(DATA_PATH "pcs-todo.txt", "r");
                char buffer1read[500];                                  
                bool start1Printing = false;
                char complete[10] = "[+]";
                char pending[10] = "[!]";
                char Work[20] = "WORK";
                char School[20] = "SCHOOL";
                char Fitness[20] = "FITNESS";
                char Personal[20] = "PERSONAL";
                char Low[20] = "LOW";
                char Medium[20] = "MEDIUM";
                char High[20] = "HIGH";
                char Urgent[20] = "URGENT";
                char readcomplete[500];

                char *startprio = strchr(text, '[');
                char *endprio = strchr(text, ']');
                char *startcat = strchr(text, '(');
                char *endcat = strrchr(text, ')');
                char *startstat =  strrchr(text, '{');
                char *endstat = strrchr(text, '}');

                char prioType[20]; 
                char catType[20]; 
                char statType[20]; 
                char prioTypeformat[20];
                char catTypeformat[20];
                char statTypeformat[20];

                char resetnull[1] = "\0";

                if (startprio && endprio) {
                    int len = endprio - startprio - 1;
                    if (len > 0 && len < 20) {
                        strncpy(prioType, startprio + 1, len);
                        prioType[len] = '\0';
                        sprintf(prioTypeformat, "%s", prioType);
                        to_uppercase(prioTypeformat);
                    } 
                    if (strstr(prioTypeformat, "LOW") == NULL && strstr(prioTypeformat, "MEDIUM") == NULL && strstr(prioTypeformat, "HIGH") == NULL && strstr(prioTypeformat, "URGENT") == NULL) {
                        sprintf(prioTypeformat, "INVALID");
                    }
                }

                if (startcat && endcat) {
                    int len = endcat - startcat - 1;
                    if (len > 0 && len < 20) {
                        strncpy(catType, startcat + 1, len);
                        catType[len] = '\0';
                        sprintf(catTypeformat, "%s", catType);
                        to_uppercase(catTypeformat);
                    } 
                    if (strstr(catTypeformat, "WORK") == NULL && strstr(catTypeformat, "SCHOOL") == NULL && strstr(catTypeformat, "PERSONAL") == NULL && strstr(catTypeformat, "FITNESS") == NULL) {
                        sprintf(catTypeformat, "INVALID");
                    }
                }

                if (startstat && endstat) {
                    int len = endstat - startstat - 1;
                    if (len > 0 && len < 20) {
                        strncpy(statType, startstat + 1, len);
                        statType[len] = '\0';
                    }
                    if (strstr(statType, "resolved") != NULL) {
                        sprintf(statTypeformat, "[+]", statType);
                    } else if (strstr(statType, "pending") != NULL) {
                        sprintf(statTypeformat, "[!]", statType);
                    } else {
                        sprintf(statTypeformat, "INVALID", statType);
                    }
                }

                if (strstr(prioTypeformat, "INVALID") != NULL) {
                    printf( "%s" "\aSYNTAX ERROR: '%s' is not recognized as a persistent task priority.\n"  "%s", ERR_C, prioType, MAIN_C);
                    printf( "%s" "\nAdvise: Use 'urgent', 'high', 'medium', or 'low'.\n"  "%s", SUCCESS_C, MAIN_C);
                } else if (strstr(catTypeformat, "INVALID") != NULL) {
                    printf( "%s" "\aSYNTAX ERROR: '%s' is not recognized as a persistent task category.\n"  "%s", ERR_C, catType, MAIN_C);
                    printf( "%s" "\nAdvise: Use 'personal', 'work', 'school', or 'fitness'.\n"  "%s", SUCCESS_C, MAIN_C);
                } else if (strstr(statTypeformat, "INVALID") != NULL) {
                    printf( "%s" "\aSYNTAX ERROR: '%s' is not recognized as a persistent task status.\n"  "%s", ERR_C, statType, MAIN_C);
                    printf( "%s" "\nAdvise: Use 'resolved' or 'pending'.\n"  "%s", SUCCESS_C, MAIN_C);
                } else {
                    int foundCount = 0;
                    
                    while (fgets(buffer1read, sizeof(buffer1read), ptR) != NULL) {
                        bool passPrio = (startprio == NULL) || (strstr(buffer1read, prioTypeformat) != NULL);
                        bool passCat  = (startcat == NULL)  || (strstr(buffer1read, catTypeformat) != NULL);
                        bool passStat = (startstat == NULL) || (strstr(buffer1read, statTypeformat) != NULL);

                        if (passPrio && passCat && passStat) {
                            
                            if (foundCount == 0) {
								printf("\n"  "%s"  "=====[-ACTIVE-GRID-PERSISTENT-OBJECTIVES-]====="  "%s" "\n", ACCENT_C, MAIN_C);
							}
                            foundCount++;
                            const char* colorCode = theme->main_color;

                            if (strstr(buffer1read, "[+]"))         colorCode = SUCCESS_C;
                            else if (strstr(buffer1read, "URGENT")) colorCode = COLOR_STATUS_URGENT;
                            else if (strstr(buffer1read, "HIGH"))   colorCode = COLOR_STATUS_HIGH;
                            else if (strstr(buffer1read, "MEDIUM")) colorCode = COLOR_STATUS_MEDIUM;
                            else if (strstr(buffer1read, "LOW"))    colorCode = COLOR_STATUS_LOW;

                            printf("%s%s"  "%s", colorCode, buffer1read, MAIN_C);
                        }
                    }

                    if (foundCount == 0) {
                        printf("%s\aERROR: No matching sectors found in current Grid depth.%s\n", ERR_C, MAIN_C);
                    }
                }
                strncpy(prioType, resetnull, sizeof(resetnull));
                strncpy(catType, resetnull, sizeof(resetnull));
                fclose(ptR);
            } else if (strstr(text, completetodoID) != NULL) { //todo completion 
                char *filePath = DATA_PATH "pcs-todo.txt";
                char *tempPath = DATA_PATH "temp.txt";

                FILE *pR = fopen(filePath, "r");
                FILE *pT = fopen(tempPath, "w");

                bool found = false;
                char bufferread[500] = "";

                if (pR == NULL || pT == NULL) {
                    printf( "%s" "\aCRITICAL ERROR: Memory Sector Unreachable. Unable to establish a link to the task database.\n"  "%s", ERR_C, MAIN_C);
                    if (pR) fclose(pR);
                    if (pT) fclose(pT);
                    continue; 
                }

                // Check file size and reset pointer
                fseek(pR, 0, SEEK_END);
                long size = ftell(pR);
                rewind(pR); // CRITICAL: Resets the needle to the start after measuring size

                if (size == 0) {
                    printf("%s\aERROR: Data stream returned 'NULL'.\n", ERR_C);
                    printf("%sADVICE: Execute 'pcs add todo' to establish a data-anchor.%s\n", SUCCESS_C, MAIN_C);
                    fclose(pR);
                    fclose(pT);
                    remove(tempPath);
                } else {
                    char targetIDStr[10] = "";
                    printf(">> Specify Target ID for Resolution Protocol: ");
                    fgets(targetIDStr, sizeof(targetIDStr), stdin);
                    targetIDStr[strcspn(targetIDStr, "\n")] = '\0';

                    char searchID[20];
                    sprintf(searchID, "%s|", targetIDStr);

                    if (strlen(targetIDStr) == 0) {
                        printf( "%s" "\aTRACE ERROR: Target ID null. Re-input valid identifier for Grid processing.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else if (strstr(targetIDStr, "cancel") != NULL) {
                        printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else {
                        int i;
                        char check;
                        int matches1 = sscanf(targetIDStr, "%d %c", &i, &check);

                        if (matches1 == 1) {
                            if (i >= 1 && i <= 99999999) {
                                while (fgets(bufferread, sizeof(bufferread), pR) != NULL) { 
                                    if (!found && strstr(bufferread, searchID) == bufferread && strcmp(targetIDStr, "0") != 0) {
                                        found = true;
                                        char *ptr = strstr(bufferread, "[!]");

                                        if (ptr != NULL) {
                                            *(ptr + 1) = '+';
                                            printf( "%s" "PROTOCOL COMPLETE: Persistent Task #%s neutralized. Cycle synchronized.\n"  "%s", SUCCESS_C, targetIDStr, MAIN_C);
                                        } else {
                                            printf( "%s" "\aREDUNDANCY ALERT: Persistent Task #%s is already offline. Target state is already resolved.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                        }
                                    } 
                                    fprintf(pT, "%s", bufferread);
                                } 

                                fclose(pR);
                                fclose(pT);

                                if (found) {
                                    remove(filePath);
                                    if (rename(tempPath, filePath) != 0) {
                                        printf( "%s" "\aCRITICAL Failure: Bit-stream corruption detected. Unable to write changes to Sector Data.", ERR_C);
                                    }
                                } else {
                                    remove(tempPath);
                                    if (strcmp(targetIDStr, "0") == 0) {
                                        printf( "%s" "\aTRACE FAILURE: ID #%s not detected on the Grid.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                    } else {
                                        printf( "%s" "\aTRACE FAILURE: ID #%s not detected on the Grid.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                    }
                                }
                            }
                        } else {
                            printf( "%s" "\aSYNTAX ERROR: Invalid Bit-Stream. Data must be an integer value"  "%s\n", ERR_C, MAIN_C);
                            fclose(pR);
                            fclose(pT);
                            remove(tempPath);
                        }
                    }
                }
            } else if (strstr(text, resettodoID) != NULL) { //todo reset 
                char *filePath = DATA_PATH "pcs-todo.txt";
                char *tempPath = DATA_PATH "temp.txt";

                FILE *pR = fopen(filePath, "r");
                FILE *pT = fopen(tempPath, "w");

                bool found = false;
                char bufferread[500] = "";

                if (pR == NULL || pT == NULL) {
                    printf( "%s" "\aCRITICAL ERROR: AccelX Sector Unreachable. Unable to establish a handshake with the task database.\n"  "%s", ERR_C, MAIN_C);
                    if (pR) fclose(pR);
                    if (pT) fclose(pT);
                    continue; 
                }

                // Check file size and reset pointer
                fseek(pR, 0, SEEK_END);
                long size = ftell(pR);
                rewind(pR); // CRITICAL: Resets cursor to line 1, char 0 after measurement

                if (size == 0) {
                    printf("%s\aERROR: Data stream returned 'NULL'.\n", ERR_C);
                    printf("%sADVICE: Execute 'pcs add todo' to establish a data-anchor.%s\n", SUCCESS_C, MAIN_C);
                    fclose(pR);
                    fclose(pT);
                    remove(tempPath);
                } else {
                    char targetIDStr[10] = "";
                    printf(">> Identify Target ID for Re-initialization Protocol: ");
                    fgets(targetIDStr, sizeof(targetIDStr), stdin);
                    targetIDStr[strcspn(targetIDStr, "\n")] = '\0';

                    char searchID[20];
                    sprintf(searchID, "%s|", targetIDStr);

                    if (strlen(targetIDStr) == 0) {
                        printf( "%s" "\aTRACE ERROR: Target ID null. Re-input valid identifier for Grid processing.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else if (strstr(targetIDStr, "cancel") != NULL) {
                        printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else {
                        int i;
                        char check;
                        int matches1 = sscanf(targetIDStr, "%d %c", &i, &check);

                        if (matches1 == 1) {
                            if (i >= 1 && i <= 99999999) {
                                while (fgets(bufferread, sizeof(bufferread), pR) != NULL) { 
                                    if (!found && strstr(bufferread, searchID) == bufferread && strcmp(targetIDStr, "0") != 0) {
                                        found = true;
                                        char *ptr = strstr(bufferread, "[+]");

                                        if (ptr != NULL) {
                                            *(ptr + 1) = '!'; // Flips the bit back to active status
                                            printf( "%s" "\aGRID ALERT: Persistent Task #%s re-energized. Entry has been pushed back to the Active Buffer.\n"  "%s", SUCCESS_C, targetIDStr, MAIN_C);
                                        } else {
                                            printf( "%s" "\aLOGIC ERROR: Persistent Task #%s bit-state mismatch. Sector is either already active or pending initialization.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                        }
                                    } 
                                    fprintf(pT, "%s", bufferread);
                                } 
                                
                                fclose(pR);
                                fclose(pT);

                                if (found) {
                                    remove(filePath);
                                    if (rename(tempPath, filePath) != 0) {
                                        printf( "%s" "\aCRITICAL FAILURE: AccelX Write-Shield active. Unable to commit changes", ERR_C);
                                    }
                                } else {
                                    remove(tempPath);
                                    if (strcmp(targetIDStr, "0") == 0) {
                                        printf( "%s" "\aTRACE FAILURE: ID #%s not detected on the Grid.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                    } else {
                                        printf( "%s" "\aTRACE FAILURE: ID #%s not detected on the Grid.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                    }
                                }
                            }
                        } else {
                            printf( "%s" "\aSYNTAX ERROR: Invalid Bit-Stream. Data must be an integer value"  "%s\n", ERR_C, MAIN_C);
                            fclose(pR);
                            fclose(pT);
                            remove(tempPath);
                        }
                    }
                }
            } else if (strstr(text, deletetodoID) != NULL) { //todo delete 
                char *filePath = DATA_PATH "pcs-todo.txt";
                char *tempPath = DATA_PATH "temp.txt";

                FILE *pR = fopen(filePath, "r");
                FILE *pT = fopen(tempPath, "w");

                bool found = false;
                char bufferread[500] = "";

                if (pR == NULL || pT == NULL) {
                    printf( "%s" "\aCRITICAL ERROR: AccelX Sector Unreachable. Unable to establish a handshake with the task database.\n"  "%s", ERR_C, MAIN_C);
                    if (pR) fclose(pR);
                    if (pT) fclose(pT);
                    continue; 
                }

                // Check file size and reset pointer
                fseek(pR, 0, SEEK_END);
                long size = ftell(pR);
                rewind(pR); // CRITICAL: Resets cursor to start after size check

                if (size == 0) {
                    printf("%s\aERROR: Data stream returned 'NULL'.\n", ERR_C);
                    printf("%sADVICE: Execute 'pcs add todo' to establish a data-anchor.%s\n", SUCCESS_C, MAIN_C);
                    fclose(pR);
                    fclose(pT);
                    remove(tempPath);
                } else {
                    char targetIDStr[10] = "";
                    printf(">> Identify Target ID for Data Derezz Protocol: ");
                    fgets(targetIDStr, sizeof(targetIDStr), stdin);
                    targetIDStr[strcspn(targetIDStr, "\n")] = '\0';

                    char searchID[20];
                    sprintf(searchID, "%s|", targetIDStr);

                    if (strlen(targetIDStr) == 0) {
                        printf( "%s" "\aTRACE ERROR: Target ID null. Re-input valid identifier for Grid processing.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else if (strstr(targetIDStr, "cancel") != NULL) {
                        printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                        fclose(pR);
                        fclose(pT);
                        remove(tempPath);
                    } else {
                        int i;
                        char check;
                        int matches1 = sscanf(targetIDStr, "%d %c", &i, &check);

                        if (matches1 == 1) {
                            if (i >= 1 && i <= 99999999) {
                                while (fgets(bufferread, sizeof(bufferread), pR) != NULL) { 
                                    // Logic: If ID matches, we mark as found and SKIP the fprintf to the temp file
                                    if (!found && strstr(bufferread, searchID) == bufferread && strcmp(targetIDStr, "0") != 0) {
                                        found = true;
                                        printf( "%s" "PROTOCOL SUCCESS: Persistent task #%s has successfully been derezzed.\n"  "%s", SUCCESS_C, targetIDStr, MAIN_C);
                                        continue; 
                                    }
                                    fprintf(pT, "%s", bufferread);
                                }
                                
                                fclose(pR);
                                fclose(pT);

                                if (found) {
                                    remove(filePath);
                                    if (rename(tempPath, filePath) != 0) {
                                        printf( "%s" "\aCRITICAL FAILURE: AccelX Write-Shield active. Unable to commit changes", ERR_C);
                                    }
                                } else {
                                    remove(tempPath);
                                    if (strcmp(targetIDStr, "0") == 0) {
                                        printf( "%s" "\aTRACE FAILURE: ID #%s not detected on the Grid.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                    } else {
                                        printf( "%s" "\aTRACE FAILURE: ID #%s not detected on the Grid.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                                    }
                                }
                            }
                        } else {
                            printf( "%s" "\aSYNTAX ERROR: Invalid Bit-Stream. Data must be an integer value"  "%s\n", ERR_C, MAIN_C);
                            fclose(pR);
                            fclose(pT);
                            remove(tempPath);
                        }
                    }
                }
            } else if (strstr(text, purgetodoID) != NULL) { //purge todo 
                char *filePath = DATA_PATH "pcs-todo.txt";

                FILE *pR = fopen(filePath, "w");

                char YorN[10] = "";
                char blankbuffer[] = "";
                printf("%s\aAre you sure you want to commit this action? This cannot be undone. Y/N: ", ERR_C);
                fgets(YorN, sizeof(YorN), stdin);
                YorN[strcspn(YorN, "\n")] = '\0';

                if (strstr(YorN, "Y") != NULL) {
                    sprintf(blankbuffer, "\0", blankbuffer); 
                    fprintf(pR, "%s", blankbuffer);
                    fclose(pR);

                    printf( "%s" "\nSYSTEM MAINTENANCE: Executing Sector Purge"  "%s", ERR_C, MAIN_C);
                    wait_seconds(0.8);
                    printf( "%s" "."  "%s", ERR_C, MAIN_C);
                    wait_seconds(0.8);
                    printf( "%s" "."  "%s", ERR_C, MAIN_C);
                    wait_seconds(0.8);
                    printf( "%s" ".\n"  "%s", ERR_C, MAIN_C);
                    wait_seconds(0.8);

                    printf( "%s"">>> DATA STREAM CLEARED: 0x00h bits returned to the Grid.\n"  "%s", SUCCESS_C, MAIN_C);
                    wait_seconds(0.8);
                    printf( "%s" "\nPROTOCOL COMPLETE: Todo data stream has been fully de-materialized.\n"  "%s", SUCCESS_C, MAIN_C);
                } else {
                    printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                    fclose(pR);
                }
            } else if (strstr(text, edituserdataID) != NULL) { //edit user data +++
                char *filePath = DATA_PATH "pcs-user-data.txt";
                char *tempPath = DATA_PATH "temp.txt";

                bool found = false;
                char bufferread[500] = "";
                int page = 1;

                char targetIDStr[10] = "";
                char DataInput[100] = "";
                char searchID[20];

                bool overallChanged = false;

                FILE *datafile = fopen(filePath, "r");

                if (datafile == NULL) {
                    printf("%s\aCRITICAL ERROR: AccelX Sector Unreachable. Unable to establish a handshake with the task database.\n", ERR_C);
                    fclose(datafile);
                    continue;
                }
                fseek(datafile, 0, SEEK_END);
                if (ftell(datafile) == 0) {
                    printf("%s\aTRACE FAILURE: User data sector returned 'NULL' in the Grid database.\n", ERR_C);
                    fclose(datafile); 
                    continue;
                } else {
                    fclose(datafile); 
                }
                rewind(datafile);

                while(1) {
                    int reset = 1; //if reset = 1 then continue ; if reset = 2 then reset
                    
                    switch(page) {
                    case 1:
                        printf("[=-----] 1/6 >> Enter Username: ");
                        fgets(DataInput, sizeof(DataInput), stdin);
                        DataInput[strcspn(DataInput, "\n")] = '\0';
                        break;
                    case 2:
                        printf("[==----] 2/6 >> Enter Sector (Country): ");
                        fgets(DataInput, sizeof(DataInput), stdin);
                        DataInput[strcspn(DataInput, "\n")] = '\0';
                        break;
                    case 3:
                        printf("[===---] 3/6 >> Enter Date Of Birth (MM/DD/YY): ");
                        fgets(DataInput, sizeof(DataInput), stdin);
                        DataInput[strcspn(DataInput, "\n")] = '\0';

                        int m, d, y;
                        int matches = sscanf(DataInput, "%d/%d/%d", &m, &d, &y);

                        if (matches == 3) {
                            if (m >= 1 && m <= 12 && d >= 1 && d <= 31 && y >= 0 && y <= 99) {
                                sprintf(DataInput, "%02d/%02d/%02d", m, d, y);
                                reset = 1;
                                break; 
                            } else {
                                printf( "%s" "\aSYNTAX ERROR: Date values out of bounds (1-12/1-31/00-99).\n\n"  "%s", ERR_C, MAIN_C);
                                reset = 2;
                            }
                        } else if (strstr(DataInput, "next") != NULL || strstr(DataInput, "prev") != NULL || strstr(DataInput, "exit") != NULL) {
                            break;
                        } else {
                            printf( "%s" "\aSYNTAX ERROR: Invalid Bit-Stream. Use MM/DD/YY format.\n\n"  "%s", ERR_C, MAIN_C);
                            reset = 2;
                        }
                        break;
                    case 4:
                        printf("[====--] 4/6 >> Enter Gender: ");
                        fgets(DataInput, sizeof(DataInput), stdin);
                        DataInput[strcspn(DataInput, "\n")] = '\0';
                        break;
                    case 5:
                        printf("[=====-] 5/6 >> Enter Height (cm): ");
                        fgets(DataInput, sizeof(DataInput), stdin);
                        DataInput[strcspn(DataInput, "\n")] = '\0';
                        
                        int i;
                        char check;
                        int matches1 = sscanf(DataInput, "%d %c", &i, &check);

                        if (matches1 == 1) {
                            if (i >= 1 && i <= 99999999) {
                                sprintf(DataInput, "%s cm", DataInput);
                                reset = 1;
                                break; 
                            } else {
                                printf( "%s" "\aSYNTAX ERROR: Invalid Bit-Stream. Data must be an integer value"  "%s", ERR_C, MAIN_C);
                                reset = 3;
                            }
                        } else if (strstr(DataInput, "next") != NULL || strstr(DataInput, "prev") != NULL || strstr(DataInput, "exit") != NULL) {
                            break;
                        } else {
                            printf( "%s" "\aSYNTAX ERROR: Invalid Bit-Stream. Data must be an integer value\n\n"  "%s", ERR_C, MAIN_C);
                            reset = 3;
                        }
                        break;
                    case 6:
                        printf("[======] 6/6 >> Enter Weight (kg): ");
                        fgets(DataInput, sizeof(DataInput), stdin);
                        DataInput[strcspn(DataInput, "\n")] = '\0';

                        int i1;
                        char check1;
                        int matches2 = sscanf(DataInput, "%d %c", &i1, &check);

                        if (matches2 == 1) {
                            if (i1 >= 1 && i1 <= 99999999) {
                                sprintf(DataInput, "%s kg", DataInput);
                                reset = 1;
                                break; 
                            } else {
                                printf( "%s" "\aSYNTAX ERROR: Invalid Bit-Stream. Data must be an integer value"  "%s", ERR_C, MAIN_C);
                                reset = 4;
                            }
                        } else if (strstr(DataInput, "next") != NULL || strstr(DataInput, "prev") != NULL || strstr(DataInput, "exit") != NULL) {
                            break;
                        } else {
                            printf( "%s" "\aSYNTAX ERROR: Invalid Bit-Stream. Data must be an integer value\n\n"  "%s", ERR_C, MAIN_C);
                            reset = 4;
                        }
                        break;
                    }

                    sprintf(searchID, "%d|", page);
                    
                    if (strlen(DataInput) == 0 && reset == 1) {
                        printf( "%s" "\aTRACE ERROR: Target ID null. Re-input valid identifier for Grid processing.\n"  "%s", ERR_C, MAIN_C);
                    } else if (strstr(DataInput, "exit") != NULL) {
                        if (overallChanged) {
                            char timestamp[30];
                            time_t now = time(NULL);
                            struct tm *t = localtime(&now);
                            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
            
                            sprintf(searchID, "7|");

                            FILE *pR2 = fopen(filePath, "r");
                            FILE *pT2 = fopen(tempPath, "w");

                            if (pR2 != NULL && pT2 != NULL) {
                                while (fgets(bufferread, sizeof(bufferread), pR2) != NULL) {
                                    if (strstr(bufferread, searchID) == bufferread) {
                                        fprintf(pT2, "7| %s\n", timestamp);
                                    } else {
                                        fprintf(pT2, "%s", bufferread);
                                    }
                                }
                                fclose(pR2); 
                                fclose(pT2);
                                remove(filePath);
                                rename(tempPath, filePath);
                                }
                        }
                        printf( "%s" "PROTOCOL SUCCESS: Trace '%s' Manifest Synced successfully. Connection Terminated.\n"  "%s", SUCCESS_C, userHexID, MAIN_C);
                        break;
                    } else if (strstr(DataInput, "next") != NULL && reset == 1) {
                        page++;
                        if (page == 7) {
                            page = 1;
                        }
                    } else if (strstr(DataInput, "prev") != NULL && reset == 1) {
                        page--;
                        if (page == 0) {
                            page = 6;
                        }
                    } else if (reset == 1 && strlen(DataInput) < 50) {
                        bool isChanged = false;
                        char currentDiskValue[100] = "";
                        FILE *pCheck = fopen(filePath, "r");
                        if (pCheck) {
                            while (fgets(bufferread, sizeof(bufferread), pCheck)) {
                                if (strstr(bufferread, searchID) == bufferread) {
                                    char *ptr = strchr(bufferread, '|');
                                    if (ptr) {
                                        ptr += 2; // Skip "| "
                                        strcpy(currentDiskValue, ptr);
                                        currentDiskValue[strcspn(currentDiskValue, "\n")] = '\0';
                                    }
                                    break;
                                }
                            }
                            fclose(pCheck);
                        }
                        if (strcmp(currentDiskValue, DataInput) != 0) isChanged = true;

                        FILE *pR = fopen(filePath, "r");
                        FILE *pT = fopen(tempPath, "w");

                        if (pR == NULL || pT == NULL) {
                            printf( "%s" "\aCRITICAL ERROR: AccelX Sector Unreachable. Unable to establish a handshake with the task database.\n"  "%s", ERR_C, MAIN_C);
                            if (pR) fclose(pR);
                            if (pT) fclose(pT);
                            continue; 
                        }

                        if (page > 1) {
                            DataInput[sizeof(DataInput)] = '\0';
                            to_uppercase(DataInput);
                            for (int i = 1; DataInput[i] != '\0'; i++) {
                                DataInput[i] = tolower((unsigned char)DataInput[i]);
                            }
                        } 
                        
                        while (fgets(bufferread, sizeof(bufferread), pR) != NULL) {
                            if (strstr(bufferread, searchID) == bufferread) {
                                found = true;
                                fprintf(pT, "%d| %s\n", page, DataInput);
                            } else {
                                fprintf(pT, "%s", bufferread);
                            }           
                        }

                        fclose(pR);
                        fclose(pT);

                        if (found) {
                            remove(filePath);
                            if (rename(tempPath, filePath) != 0) {
                                printf( "%s" "\aCRITICAL FAILURE: AccelX Write-Shield active. Unable to commit changes\n"  "%s", ERR_C, MAIN_C);
                            } else {
                                overallChanged = true;
                            }
                        } else {
                            remove(tempPath);
                            printf( "%s" "\aTRACE FAILURE: ID #%s not detected on the Grid. Entity may have been Derezzed.\n"  "%s", ERR_C, targetIDStr, MAIN_C);
                        }
                        
                        if (page == 6) {
                            printf( "%s" "PROTOCOL SUCCESS: Trace '%s' Manifest Synced successfully. Connection Terminated.\n"  "%s", SUCCESS_C, userHexID, MAIN_C);

                            if (isChanged) {
                                char timestamp[30];
                                time_t now = time(NULL);
                                struct tm *t = localtime(&now);
                                strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
                                
                                sprintf(searchID, "7|");

                                FILE *pR2 = fopen(filePath, "r");
                                FILE *pT2 = fopen(tempPath, "w");

                                if (pR2 != NULL && pT2 != NULL) {
                                    while (fgets(bufferread, sizeof(bufferread), pR2) != NULL) {
                                        if (strstr(bufferread, searchID) == bufferread) {
                                            fprintf(pT2, "7| %s\n", timestamp);
                                        } else {
                                            fprintf(pT2, "%s", bufferread);
                                        }
                                    }
                                    fclose(pR2); 
                                    fclose(pT2);
                                    remove(filePath);
                                    rename(tempPath, filePath);
                                }
                            }
                            break;
                        } else if (page < 6) {
                            page++;
                        }
                    } else if (strlen(DataInput) > 50) {
                        printf("%s\aSYNTAX ERROR: Invalid Bit-Stream. Data must be within 50 characters\n\n%s", ERR_C, MAIN_C);
                        memset(DataInput, 0, sizeof(DataInput));   
                        int c;
                        while ((c = getchar()) != '\n' && c != EOF);
                        reset = 0;
                    } else if (reset == 2) {
                        page = 3;
                    } else if (reset == 3) {
                        page = 5;
                    } else if (reset == 4) {
                        page = 6;
                    }
                }
            } else if (strstr(text, seeuserdataID) != NULL) { //see user data 
                FILE *datafile = fopen(DATA_PATH "pcs-user-data.txt", "r");
                if (datafile == NULL) {
                    printf("%s\aCRITICAL ERROR: AccelX Sector Unreachable. Unable to establish a handshake with the task database.\n", ERR_C);
                    fclose(datafile);
                    continue;
                }
                
                fseek(datafile, 0, SEEK_END);
                if (ftell(datafile) == 0) {
                    printf("%s\aTRACE FAILURE: User data sector returned 'NULL' in the Grid database.\n", ERR_C);
                    fclose(datafile); 
                    continue;
                }
                rewind(datafile);

                int nameID = 1; char nameextract[100] = "";
                int uidID = 0; char UIDextract[100] = "";
                int sectorID = 2; char sectorextract[100] = "";
                int dateofbirthID = 3; char dateofbirthextract[100] = "";
                int genderID = 4; char genderextract[100] = "";
                int heightID = 5; char heightextract[100] = "";
                int weightID = 6; char weightextract[100] = "";
                int lastsyncID = 7; char lastsyncextract[100] = "";

                char line[256];
                int currentID;
                char tempValue[100];

                while (fgets(line, sizeof(line), datafile)) {
                    if (sscanf(line, "%d| %[^\n]", &currentID, tempValue) == 2) {
                        switch (currentID) {
                            case 0: strcpy(UIDextract, tempValue); break;
                            case 1: strcpy(nameextract, tempValue); break;
                            case 2: strcpy(sectorextract, tempValue); break;
                            case 3: strcpy(dateofbirthextract, tempValue); break;
                            case 4: strcpy(genderextract, tempValue); break;
                            case 5: strcpy(heightextract, tempValue); break;
                            case 6: strcpy(weightextract, tempValue); break;
                            case 7: strcpy(lastsyncextract, tempValue); break;
                        }
                    }
                }
                fclose(datafile);
                
                printf( "%s"  "===========[-USER-DATA-]===========\n"  "%s", ACCENT_C, MAIN_C);
                printf("[NAME] %s\n", nameextract);
                printf("[UID] %s\n", UIDextract);
                printf( "%s"  "===================================\n"  "%s", ACCENT_C, MAIN_C);
                printf("[SECTOR] %s\n", sectorextract);
                printf( "%s"  "===================================\n"  "%s", ACCENT_C, MAIN_C);
                printf("[DATE OF BIRTH] %s\n", dateofbirthextract);
                printf("[GENDER] %s\n", genderextract);
                printf("[HEIGHT] %s\n", heightextract);
                printf("[WEIGHT] %s\n", weightextract);
                printf( "%s"  "===================================\n"  "%s", ACCENT_C, MAIN_C);
                printf("[LAST SYNC] %s\n", lastsyncextract);
                printf( "%s"  "===================================\n"  "%s", ACCENT_C, MAIN_C);
            } else if (strstr(text, themeswitchID) != NULL) { //switch themes
                int targetthemeID;
				show_theme_menu();

				if (scanf("%d", &targetthemeID) == 1) {
                    while(getchar() != '\n');
                    rectified = 3;
                    if (targetthemeID < 0 || targetthemeID > 8) {
                        printf("\n%s\aSYNTAX ERROR: Invalid Protocol ID. Operation Aborted.%s\n", ERR_C, MAIN_C);
					} else if (targetthemeID == 0) {
                        printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                    } else {
                        char *filePath = DATA_PATH "pcs-user-data.txt";
						char *tempPath = DATA_PATH "temp.txt";

						FILE *pR = fopen(filePath, "r");
						FILE *pT = fopen(tempPath, "w");

                        fseek(pR, 0, SEEK_END);
                        if (ftell(pR) == 0) {
                            printf("%s\aTRACE FAILURE: Theme sector not detected in the Grid database.\n", ERR_C);
                            continue;
                        }
                        rewind(pR);

						bool found = false;
						char bufferread[500] = "";

						if (pR == NULL || pT == NULL) {
							printf("%s\aCRITICAL ERROR: AccelX Sector Unreachable. Unable to establish a handshake with the database.\n%s", ERR_C, MAIN_C);
							if (pR) fclose(pR);
							if (pT) fclose(pT);
						} else {
							while (fgets(bufferread, sizeof(bufferread), pR) != NULL) {
								if (!found && strncmp(bufferread, "8|", 2) == 0) {
									found = true;
									fprintf(pT, "8| %d\n", targetthemeID);
								} else {
									fprintf(pT, "%s", bufferread);
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
                                    
									break;
								}
							} else {
								remove(tempPath);
								printf("%s\aTRACE FAILURE: Theme sector not detected in the Grid database.\n%s", ERR_C, MAIN_C);
							}
						}
					}                                   
				} else {
                    while(getchar() != '\n');
                    rectified = 0;
                    printf( "%s" "\aTRACE ERROR: Target ID null. Re-input valid identifier for Grid processing.\n"  "%s", ERR_C, MAIN_C);
                }
            } else if (strstr(text, rectifID) != NULL) { //full system purge
                char timestamp[30];
                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", t);
                char bufferread[500] = "";

                rectified = 2;

                printf("\n%s\aCRITICAL WARNING: INITIATING SYSTEM RECTIFICATION.\n", monolith_theme.error_color);
                printf("This will derez all archives, logs, and user identity fragments.\n");
                printf("Input Master Override (Y/N): ");

                char confirm;
                scanf(" %c", &confirm);

                if (confirm == 'Y' || confirm == 'y') {
                    char sign1 = '%';
                    while(getchar() != '\n');

                    wait_seconds(1);
                    printf("\n%sSYSTEM MAINTENANCE: Executing Global Sector Purge", ERR_C); showdots();
                    wait_seconds(1);
                    printf("\x1B[2J\x1B[H"); printf("%s%s", reset.main_color, reset.background_color);

                    FILE *a = fopen(DATA_PATH "pcs-tasks.txt", "w");
                    if (a != NULL) {
                        fclose(a);
                    }
                    FILE *ax = fopen(DATA_PATH "pcs-archive-tasks.txt", "w");
                    if (ax != NULL) {
                        fclose(ax);
                    }
                    FILE *b = fopen(DATA_PATH "pcs-logs.txt", "w");
                    if (b != NULL) {
                        fclose(b);
                    }
                    FILE *bx = fopen(DATA_PATH "pcs-archive-logs.txt", "w");
                    if (bx != NULL) {
                        fclose(bx);
                    }
                    FILE *c = fopen(DATA_PATH "pcs-todo.txt", "w");
                    if (c != NULL) {
                        fclose(c);
                    }
                    FILE *pT = fopen(DATA_PATH "temp.txt", "w");
				    FILE *pR = fopen(DATA_PATH "pcs-user-data.txt", "r");

				    if (pT == NULL || pR == NULL) {
					    if (pT) fclose(pT);
					    if (pR) fclose(pR);
					    printf("CRITICAL ERROR: File access denied.\n");
					    continue;
				    }

				    fprintf(pT, "0| %s\n", userHexID);
				    fprintf(pT, "1| Operator\n2| Country\n3| MM/DD/YY\n4| Gender\n5| Height\n6| Weight\n");
				    fprintf(pT, "7| %s\n", timestamp);
				    fprintf(pT, "8| 1\n");

				    while (fgets(bufferread, sizeof(bufferread), pR) != NULL) {
					    int prefix = -1;
					    if (sscanf(bufferread, "%d|", &prefix) == 1) {
						    if (prefix >= 0 && prefix <= 8) {
							    continue; 
						    }
					    }
					    fprintf(pT, "%s", bufferread);
				    }

				    fclose(pR);
				    fclose(pT);

				    remove(DATA_PATH "pcs-user-data.txt");
				    rename(DATA_PATH "temp.txt", DATA_PATH "pcs-user-data.txt");

                    showdots(); wait_seconds(2); printf("\x1B[2J\x1B[H");
                    printf("System Rectification Status: "); wait_seconds(3);
                    printf("100%c complete.\n\n", sign1); wait_seconds(2);
                    printf("Rebooting System"); showdots(); wait_seconds(3);
                    printf("\x1B[2J\x1B[H"); wait_seconds(1);

                    break;
                } else {
                    while(getchar() != '\n');
                    printf( "%s" "OPERATION ABORTED: User intervention detected. Rectification terminated. Data integrity preserved.\n"  "%s", ERR_C, MAIN_C);
                    rectified = 0;
                }
            } else if (strstr(text, purgearchivesID) != NULL) { //purge all saves in archives
                int targetarchiveID;
                show_archives_menu();
                char targetarchivename[50] = "";

                if (scanf("%d", &targetarchiveID) == 1) {
                    
                    while(getchar() != '\n');         
                    if (targetarchiveID < 0 || targetarchiveID > 2) {
						printf("\n%s\aSYNTAX ERROR: Invalid Protocol ID. Operation Aborted.%s\n", ERR_C, MAIN_C);
					} else if (targetarchiveID == 0) {
                        printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                    } else {
                        char YorN[10] = "";
                        printf("%sAre you sure you want to commit this action? This cannot be undone. Y/N: ", ERR_C);
                        fgets(YorN, sizeof(YorN), stdin);
                        YorN[strcspn(YorN, "\n")] = '\0';

                        if (strstr(YorN, "Y") != NULL || strstr(YorN, "y") != NULL) {
                            if (targetarchiveID == 1) {
                                char *filePath1 = DATA_PATH "pcs-archive-tasks.txt";
                                FILE *pR1 = fopen(filePath1, "w"); fclose(pR1);
                                strcpy(targetarchivename, "tasks");
                            } else if (targetarchiveID == 2) {
                                char *filePath2 = DATA_PATH "pcs-archive-logs.txt";
                                FILE *pR2 = fopen(filePath2, "w"); fclose(pR2);
                                strcpy(targetarchivename, "logs");
                            }
                            printf( "%s" "\nSYSTEM MAINTENANCE: Executing Sector Purge"  "%s", ERR_C, MAIN_C);
                            wait_seconds(0.8);
                            printf( "%s" "."  "%s", ERR_C, MAIN_C);
                            wait_seconds(0.8);
                            printf( "%s" "."  "%s", ERR_C, MAIN_C);
                            wait_seconds(0.8);
                            printf( "%s" ".\n"  "%s", ERR_C, MAIN_C);
                            wait_seconds(0.8);

                            printf( "%s"">>> DATA STREAM CLEARED: 0x00h bits returned to the Grid.\n"  "%s", SUCCESS_C, MAIN_C);
                            wait_seconds(0.8);
                            printf( "%s" "\nPROTOCOL COMPLETE: Archived %s data stream has been fully de-materialized.\n"  "%s", SUCCESS_C, targetarchivename, MAIN_C);
                        } else {
                            printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                        }
                    }
                } else {
                    printf( "%s" "\aTRACE ERROR: Target ID null. Re-input valid identifier for Grid processing.\n"  "%s", ERR_C, MAIN_C);
                }
            } else if (strstr(text, seearchivesID) != NULL) { //see available archived dates
                int targetarchiveID;
                show_archives_menu();

                if (scanf("%d", &targetarchiveID) == 1) {
                    while(getchar() != '\n');
                    if (targetarchiveID < 0 || targetarchiveID > 2) {
						printf("\n%s\aSYNTAX ERROR: Invalid Protocol ID. Operation Aborted.%s\n", ERR_C, MAIN_C);
					} else if (targetarchiveID == 0) {
                        printf( "%s" "OPERATION ABORTED: User intervention detected. Task sequence terminated.\n"  "%s", ERR_C, MAIN_C);
                    } else {
                        int foundCount = 0;
					    char buffer[512] = "";
                        FILE *ptR1 = fopen(DATA_PATH "pcs-archive-tasks.txt", "r");
                        FILE *ptR2 = fopen(DATA_PATH "pcs-archive-logs.txt", "r");

                        if (targetarchiveID == 1) {
                            while (fgets(buffer, sizeof(buffer), ptR1) != NULL) {
						        if (strstr(buffer, "0|") == buffer) {
							        if (foundCount == 0) {
								        printf("\n%s====[-ARCHIVED-GRID-OBJECTIVES-]=====%s\n", ACCENT_C, MAIN_C);
							        }
							        foundCount++;
							        printf("%s%s", MAIN_C, buffer);
						        }
					        }

					        if (foundCount == 0) {
						        printf("%s\aERROR: Data stream returned 'NULL'. No protected data found in archive sectors.", ERR_C);
					        }
                        } else if (targetarchiveID == 2) {
                            while (fgets(buffer, sizeof(buffer), ptR2) != NULL) {
						        if (strstr(buffer, "0|") == buffer) {
							        if (foundCount == 0) {
								        printf("\n%s====[-ARCHIVED-GRID-LOGS-]=====%s\n", ACCENT_C, MAIN_C);
							        }
							        foundCount++;
							        printf("%s%s", MAIN_C, buffer);
						        }
					        }

					        if (foundCount == 0) {
						        printf("%s\aERROR: Data stream returned 'NULL'. No protected data found in archive sectors.", ERR_C);
					        }
                        }
                        fclose(ptR1);
                        fclose(ptR2);
                    }
                } else {
                    printf( "%s" "\aTRACE ERROR: Target ID null. Re-input valid identifier for Grid processing.\n"  "%s", ERR_C, MAIN_C);
                }
            } else if (strstr(text, repairID) != NULL) { //scans for missing files and creates new one
                int repaired = 0;
                int scanned = 1;
                FILE *Path = NULL; 
                char *filename = NULL; 

                while (1) {
                    switch (scanned) {
                        case 1: filename = DATA_PATH "pcs-tasks.txt"; break;
                        case 2: filename = DATA_PATH "pcs-archive-tasks.txt"; break;
                        case 3: filename = DATA_PATH "pcs-logs.txt"; break;
                        case 4: filename = DATA_PATH "pcs-archive-logs.txt"; break;
                        case 5: filename = DATA_PATH "pcs-todo.txt"; break;
                        default: filename = NULL; break;
                    }

                    if (scanned >= 1 && scanned <= 5 && filename != NULL) {
                        Path = fopen(filename, "r"); 

                        if (Path == NULL) {
                            Path = fopen(filename, "w");
                            if (Path != NULL) {
                                fclose(Path);
                                repaired++; 
                            }
                        } else {
                            fclose(Path);
                        }
                        scanned++; 
                    } else {
                        break;
                    }
                }

                filename = DATA_PATH "pcs-user-data.txt";
                Path = fopen(filename, "r");

                if (Path == NULL) {
                    Path = fopen(filename, "w");

                    if (Path != NULL) {
                        char timestamp[30];
                        time_t now = time(NULL);
                        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

                        fprintf(Path, "0| %s\n", userHexID);
                        fprintf(Path, "1| Operator\n2| Country\n3| MM/DD/YY\n4| Gender\n5| Height\n6| Weight\n");
                        fprintf(Path, "7| %s\n", timestamp);
                        fprintf(Path, "8| 1\n");
                        
                        fclose(Path);
                        repaired++; 
                    }
                } else {
                    fseek(Path, 0, SEEK_END);
                    if (ftell(Path) == 0) {
                        fclose(Path);
                        Path = fopen(filename, "w");

                        if (Path != NULL) {
                            char timestamp[30];
                            time_t now = time(NULL);
                            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&now));

                            fprintf(Path, "0| %s\n", userHexID);
                            fprintf(Path, "1| Operator\n2| Country\n3| MM/DD/YY\n4| Gender\n5| Height\n6| Weight\n");
                            fprintf(Path, "7| %s\n", timestamp);
                            fprintf(Path, "8| 1\n");
                            
                            fclose(Path);
                            repaired++; 
                        }
                    } else {
                        fclose(Path);
                    }
                } 

                printf("%sSYSTEM MAINTENANCE: Executing sector re-integration", ERR_C);
                wait_seconds(0.8);
                printf( "%s" "."  "%s", ERR_C, MAIN_C);
                wait_seconds(0.8);
                printf( "%s" "."  "%s", ERR_C, MAIN_C);
                wait_seconds(0.8);
                printf( "%s" ".\n"  "%s", ERR_C, MAIN_C);
                wait_seconds(1.5);
                printf("%sPROTOCOL SUCCESS: Cycle complete, %d total sectors re-integrated.\n%s", SUCCESS_C, repaired, MAIN_C);
            } else if (strstr(text, flushID) != NULL) { //soft reboot
                rectified = 3;
                break;
            } else if (strstr(text, helpID) != NULL) { //help
                gridhelpcommands();
            } else if (strstr(text, exitprogID) != NULL) { //exit
                rectified = 0;
                printf( "%s" "Connection Severed... Program Derezzed." COLOR_RESET, ERR_C);
                printf("\a");
                wait_seconds(0.5);
                printf("\a");
                wait_seconds(0.5);
                printf("\a");
                wait_seconds(0.5);
                printf("\x1B[2J\x1B[H");
                exit(0);
            } else {
                printf( "%s" "\aERROR: '%s' is not recognized as an internal or external command, operable program or batch file by the grid.\n"  "%s", ERR_C, text, MAIN_C);
            }
        }
    }

int main() {
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    #endif

    while (1) {
        PCS_Protocol();
    }
}