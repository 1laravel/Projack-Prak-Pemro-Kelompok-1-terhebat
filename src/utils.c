#include <stdio.h>
#include <stdlib.h>
#include "../include/utils.h"

void logActivity(const char *username, const char *action, const char *detail) {
    if (username == NULL || action == NULL || detail == NULL) {
        fprintf(stderr, "[ERROR] logActivity: Parameter tidak boleh NULL.\n");
        return;
    }

    FILE *logFile = fopen(LOG_FILE_PATH, "a");
    if (logFile == NULL) {
        fprintf(stderr, "[ERROR] logActivity: Gagal membuka file '%s'.\n", LOG_FILE_PATH);
        return;
    }

    fprintf(logFile, "%s | %s | %s\n", username, action, detail);
    fclose(logFile);
}

void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}