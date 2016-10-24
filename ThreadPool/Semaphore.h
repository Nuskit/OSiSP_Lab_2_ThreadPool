#ifndef SEMAPHORE_H_
#define SEMAPHORE_H_
#include "stdafx.h"
#ifndef WINDOWS_SYSTEM
	#include <semaphore.h>
	#include <limits.h>
#endif

#ifdef WINDOWS_SYSTEM
  #define SYNC_PRIMITIVE_SEMAPHORE HANDLE
	#define MAX_COUNT_SEMAPHORE 1000
#else
  #define SYNC_PRIMITIVE_SEMAPHORE sem_t
	#define MAX_COUNT_SEMAPHORE SEM_VALUE_MAX
#endif

class Semaphore
{
public:
	Semaphore();
	~Semaphore();

	void wait();
  const bool wait(const ULONG milliSeconds);
	void pulse();
private:
  SYNC_PRIMITIVE_SEMAPHORE semaphore;
};
#endif
