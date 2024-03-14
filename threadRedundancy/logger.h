//C
#ifndef LOGGER_H
#define LOGGER_H

typedef enum {
    INFO,
    WARNING,
    CRITICAL
} LogLevel;

void logger(LogLevel level, const char* message);

#endif // LOGGER_H
