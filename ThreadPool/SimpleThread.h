#ifndef SIMPLETHREAD_H_
#define SIMPLETHREAD_H_
#include "stdafx.h"

#ifdef WINDOWS_SYSTEM
#define THREAD_HANDLE HANDLE
#define THREAD_RETURN_TYPE DWORD
#else
#define THREAD_HANDLE pthread_t
#define THREAD_RETURN_TYPE void*
#endif

class ThreadPoolData;
class ThreadExtraWork;
class ILogger;
class SimpleThread
{
public:
	SimpleThread(ThreadPoolData *poolData);
	~SimpleThread();
	void run(LPVOID lpParam, const bool isAddedThread = false);
	void setAliveState(const bool value=false);
	void waitAndDeleteThreadHandle();
	const volatile bool getAliveState();
private:
	ThreadExtraWork* getThread(const bool isAddedThread,LPVOID lpParam);

  ThreadPoolData* poolData;
	volatile bool isAlive;
  THREAD_HANDLE threadHandle;
};

class SimpleThreadWork
{
public:
  static THREAD_RETURN_TYPE THREAD_RETURN_CALL run(LPVOID lpParam);
private:
	SimpleThreadWork();
};
#endif
