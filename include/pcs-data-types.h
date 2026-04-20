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

    char text[1000];

    //commands
    char addtaskID[100] = "pcs add task";
    char newtasklistID[100] = "pcs new task list";
    char completetaskID[100] = "pcs resolve task";
    char resettaskID[100] = "pcs reset task";
    char seetasklistID[100] = "pcs see task";
    char deletetaskID[100] = "pcs delete task";
    char retrievetaskID[100] = "pcs retrieve task list";

    char addtodoID[100] = "pcs add todo";
    char seetodoID[100] = "pcs see todo";
    char completetodoID[100] = "pcs resolve todo";
    char resettodoID[100] = "pcs reset todo";
    char deletetodoID[100] = "pcs delete todo";
    char purgetodoID[100] = "pcs purge todo";
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

    char newlogID[100] = "pcs new log";
    char addlogID[100] = "pcs add log";
    char seelogID[100] = "pcs see log";
    char deletelogID[100] = "pcs delete log";
    char retrievelogID[100] = "pcs retrieve log";

    char seearchivesID[100] = "pcs see archives";
    char purgearchivesID[100] = "pcs purge archives";

    char helpID[100] = "pcs help";
    char exitprogID[100] = "pcs exit";

    char seeuserdataID[100] = "pcs see user data";
    char edituserdataID[100] = "pcs edit user data";

    char repairID[100] = "pcs repair";

    char rectifID[100] = "pcs rectif"; //full system purge

    char themeswitchID[100] = "pcs themes";

    char flushID[100] = "pcs flush";

    //misc
    char welcomeMsg[100] = "";
    char username[20] = "";

    //UID
    char userHexID[20] = "";
    char line[500];
    bool hasExistingID = false;

    //Theme
    Theme* theme;
    int themeID;
    char linetheme[500];

    int rectified;

#endif