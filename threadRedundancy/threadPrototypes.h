//Thread prototypes
#ifndef THREAD_PROTOS_H
#define THREAD_PROTOS_H

#include<Windows.h>

//Prototypes
DWORD WINAPI monitorThread(LPVOID lpParam);
DWORD WINAPI timerThread(LPVOID lpParam);

#endif //THREAD_PROTOS_H