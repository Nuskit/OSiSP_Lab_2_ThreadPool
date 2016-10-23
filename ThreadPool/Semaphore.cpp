#include "stdafx.h"
#include "Semaphore.h"

Semaphore::Semaphore()
{
	semaphore = CreateSemaphore(NULL, 0, MAX_COUNT_SEMAPHORE, NULL);
}

Semaphore::~Semaphore()
{
	CloseHandle(semaphore);
}

void Semaphore::wait()
{
	WaitForSingleObject(semaphore, INFINITE);
}

const bool Semaphore::wait(const DWORD milliSeconds)
{
	return WaitForSingleObject(semaphore, milliSeconds)==WAIT_TIMEOUT ? false : true;
}

void Semaphore::pulse()
{
	ReleaseSemaphore(semaphore, 1, NULL);
}

void Semaphore::pulseAll()
{
	ReleaseSemaphore(semaphore, MAX_COUNT_SEMAPHORE, NULL);
}
