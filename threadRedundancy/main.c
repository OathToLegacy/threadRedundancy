//Copyrigtht: OathToCode 2024
#include <stdio.h>
#include <windows.h>

#include "threadPrototypes.h";
#include "logger.h";
#include "SMObjects.h";

#define NUM_THREADS 5
HANDLE mutex;


DWORD WINAPI monitorThread(LPVOID lpParam) { //master thread (actual state machine)
	threadStateMachine* machines = (threadStateMachine*)lpParam;
	//Monitor the state of the threads
	while (1) {
		int allTerminated = 1;
		for (int i = 0; i < NUM_THREADS; i++) {
			switch (machines[i].currentState) {
			case RUNNING:
				printf("Thread %d is running\n", machines[i].internalThreadID);
				break;
			case WAITING:
				break;
			case FAILED:
				printf("Thread %d has failed\n", machines[i].internalThreadID);
				//Restart the thread
				machines[i].thread = CreateThread(NULL, 0, timerThread, &machines[i], 0, &machines[i].threadID);
				break;
			case TERMINATED:
				break;
			default:
				allTerminated = 0;
			}
		}
		if (allTerminated) {
			break;
		}
		Sleep(100);
	}
	return 0;
}

DWORD WINAPI timerThread(LPVOID lpParam) {
	threadStateMachine* machine  = (threadStateMachine*)lpParam;
	//Thread work here: Have the thread add and print some numbers
	for (int i = machine->workDone + 1; i < 10; i++) {
		WaitForSingleObject(mutex, INFINITE);
		printf("Thread %d: %d\n", machine->internalThreadID, i);
		fflush(stdout); //Flush buffer to ensure that the output is printed
		ReleaseMutex(mutex);
		machine->currentState = WAITING;
		Sleep(1000);
		machine->currentState = RUNNING;

		//Make thread 2 fail when it tries to print the number 5
		if (machine->internalThreadID == 2 && i == 5 && machine->failedFlag == -1) {
			machine->failedFlag = 1;
			logger(CRITICAL, "Thread 2 has failed");
			machine->currentState = FAILED;
			return 0;
		}
	}
	//Update state
	machine->currentState = TERMINATED;
	return 0;
}


int main() {
	threadStateMachine machines[NUM_THREADS];
	
	mutex = CreateMutex(NULL, FALSE, NULL);
	if (mutex == NULL) {
		logger(CRITICAL, "CreateMutex error");
		return -1;
	}
	for(int i = 0; i < NUM_THREADS; i++) {
		machines[i].workDone = -1; //Initialize workDone to -1
		machines[i].thread = CreateThread(NULL, 0, timerThread, &machines[i], 0, &machines[i].threadID);
		if (machines[i].thread != NULL) {
			machines[i].threadID = GetThreadId(machines[i].thread);
		}
		machines[i].internalThreadID = i;
		if(machines[i].internalThreadID == 2) {
			machines[i].failedFlag = -1; //-1 indicates that the thread has not failed
		}
		printf("Thread %d has been created\n", machines[i].internalThreadID);
		machines[i].currentState = RUNNING;

		if(machines[i].thread == NULL) {
			logger(WARNING, "CreateThread error");
			return -1;
		}
		logger(INFO, "Thread has been created");

	}
	// Create the monitor thread
	HANDLE monitor = CreateThread(NULL, 0, monitorThread, machines, 0, NULL);
	if (monitor == NULL) {
		logger(WARNING, "CreateMonitorThread error");
		return -1;
	}

	// Wait until all threads are done
	for (int i = 0; i < NUM_THREADS; i++) {
		WaitForSingleObject(machines[i].thread, INFINITE);
		CloseHandle(machines[i].thread);
	}

	// Wait for the monitor thread to finish
	WaitForSingleObject(monitor, INFINITE);
	CloseHandle(monitor);

	CloseHandle(mutex);

	return 0;
}