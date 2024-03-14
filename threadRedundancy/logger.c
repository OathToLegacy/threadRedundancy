//C
#include "logger.h"
#include <stdio.h>
#include <time.h>

void logger(LogLevel level, const char* message) {
    FILE* file = fopen("log.txt", "a");
    if (file == NULL) {
        printf("Error opening log file!\n");
        return;
    }

    time_t rawtime;
    struct tm timeinfo;
    char buffer[80];
    char* levelStr = "";

    switch (level) {
    case INFO:
        levelStr = "INFO";
        break;
    case WARNING:
        levelStr = "WARNING";
        break;
    case CRITICAL:
        levelStr = "CRITICAL";
        break;
    }

    time(&rawtime);
    localtime_s(&timeinfo, &rawtime);

    strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", &timeinfo);
    fprintf(file, "[%s] [%s] %s\n", buffer, levelStr, message);
    fclose(file);
}
