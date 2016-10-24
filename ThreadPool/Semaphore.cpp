#include "stdafx.h"
#include "Semaphore.h"

Semaphore::Semaphore()
{
#ifdef WINDOWS_SYSTEM
	semaphore = CreateSemaphore(NULL, 0, MAX_COUNT_SEMAPHORE, NULL);
#else
  sem_init(&semaphore,0,0);
#endif
}

Semaphore::~Semaphore()
{
#ifdef WINDOWS_SYSTEM
  CloseHandle(semaphore);
#else
  sem_destroy(&semaphore);
#endif
}

void Semaphore::wait()
{
#ifdef WINDOWS_SYSTEM
  WaitForSingleObject(semaphore, INFINITE);
#else
  sem_wait(&semaphore);
#endif
}

#define CONVERT_MILLI_TO_NANO_VALUE 1000000
const bool Semaphore::wait(const ULONG milliSeconds)
{
#ifdef WINDOWS_SYSTEM
  return WaitForSingleObject(semaphore, milliSeconds)==WAIT_TIMEOUT ? false : true;
#else
  struct timespec ts;
  clock_gettime(CLOCK_REALTIME, &ts);

  ts.tv_nsec+=milliSeconds*CONVERT_MILLI_TO_NANO_VALUE;
  ts.tv_sec+=ts.tv_nsec/CONVERT_MILLI_TO_NANO_VALUE;
  ts.tv_nsec%=CONVERT_MILLI_TO_NANO_VALUE;

  return sem_timedwait(&semaphore,&ts)==ETIMEDOUT;
#endif
}

void Semaphore::pulse()
{
#ifdef WINDOWS_SYSTEM
  ReleaseSemaphore(semaphore, 1, NULL);
#else
  sem_post(&semaphore);
#endif
}
