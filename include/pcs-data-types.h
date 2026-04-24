#ifndef PCS_DT_H
#define PCS_DT_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif
#include "pcs-themes.h"

    char userInput[10000];

    //commands
    char addtaskID[50] = "pcs add task";
    char newtasklistID[50] = "pcs new task list";
    char resolvetaskID[50] = "pcs resolve task";
    char resettaskID[50] = "pcs reset task";
    char seetasklistID[50] = "pcs see task";
    char deletetaskID[50] = "pcs delete task";
    char retrievetaskID[50] = "pcs retrieve task list";

    char addtodoID[50] = "pcs add todo";
    char seetodoID[50] = "pcs see todo";
    char resolvetodoID[50] = "pcs resolve todo";
    char resettodoID[50] = "pcs reset todo";
    char deletetodoID[50] = "pcs delete todo";
    char purgetodoID[50] = "pcs purge todo";
    //todo features
        //categories
        char tcWork[20] = "work";
        char tcSchool[20] = "school";
        char tcFitness[20] = "fitness";
        char tcPersonal[20] = "personal";
        //priorities
        char tpLow[20] = "low";
        char tpMedium[20] = "medium";
        char tpHigh[20] = "high";
        char tpUrgent[20] = "urgent";

    char newlogID[50] = "pcs new log";
    char addlogID[50] = "pcs add log";
    char seelogID[50] = "pcs see log";
    char deletelogID[50] = "pcs delete log";
    char retrievelogID[50] = "pcs retrieve log";

    char seearchivesID[50] = "pcs see archives";
    char purgearchivesID[50] = "pcs purge archives";

    char helpID[50] = "pcs help";
    char exitprogID[50] = "pcs exit";

    char seeuserdataID[50] = "pcs see user data";
    char edituserdataID[50] = "pcs edit user data";

    char repairID[50] = "pcs repair";

    char rectifID[50] = "pcs rectif"; //full system purge

    char themeswitchID[50] = "pcs themes";

    char flushID[50] = "pcs flush";

    //misc
    char welcomeMsg[50] = "";
    char username[20] = "";

    //UID
    char userHexID[20] = "";
    char line[500];
    bool hasExistingID = false;

    //Theme
    Theme* theme;
    int themeID;

    char inputColor[30] = "";
    char mainColor [30] = "";
    char accentColor [30] = "";
    char successColor [30] = "";
    char errorColor [30] = "";
    char backgroundColor [30] = "";
    int targetthemeID;
    bool continueFlag = false;
    bool breakFlag = false;

    int rectified;

#endif