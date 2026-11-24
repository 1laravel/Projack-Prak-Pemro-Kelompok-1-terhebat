#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/utils.h"

#define TIMESTAMP_LEN    20
#define TIMESTAMP_FORMAT "%Y-%m-%d %H:%M:%S"

static void getCurrentTimestamp(char *buffer, size_t bufferSize) {
    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);

    if (localTime == NULL) {
        snprintf(buffer, bufferSize, "UNKNOWN_TIME");
        return;
    }

    strftime(buffer, bufferSize, TIMESTAMP_FORMAT, localTime);
}

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

    char timestamp[TIMESTAMP_LEN];
    getCurrentTimestamp(timestamp, sizeof(timestamp));

    fprintf(logFile, "[%s] %s | %s | %s\n", timestamp, username, action, detail);
    fclose(logFile);
}

void clearScreen(void) {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}