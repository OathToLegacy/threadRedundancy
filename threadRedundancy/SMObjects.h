// State Machine Objects (SMObjects.h)

#ifndef THREAD_STATE_MACHINE_H
#define THREAD_STATE_MACHINE_H

#include <windows.h>

typedef enum {
    RUNNING,
    WAITING,
    FAILED,
    TERMINATED
} threadState;

typedef struct {
    HANDLE thread;
    DWORD threadID;
    int internalThreadID; //Custom assigned thread ID
    int failedFlag; //Mark if this thread has failed
    threadState currentState;
    int workDone;
} threadStateMachine;

DWORD WINAPI workerThread(LPVOID lpParam);

#endif // THREAD_STATE_MACHINE_H
