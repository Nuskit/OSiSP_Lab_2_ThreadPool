#include "stdafx.h"
#include "SimpleThread.h"
#include "ThreadPoolData.h"
#include "ThreadExtraWork.h"
#include "Semaphore.h"

SimpleThread::SimpleThread(ThreadPoolData * poolData) :poolData(poolData), isAlive(false)
#ifdef WINDOWS_SYSTEM
, threadHandle(nullptr)
#endif
{
}

SimpleThread::~SimpleThread()
{
  waitAndDeleteThreadHandle();
}

void SimpleThread::run(LPVOID lpParam, const bool isAddedThread)
{
  isAlive = true;
#ifdef WINDOWS_SYSTEM
  threadHandle = CreateThread(NULL, 0, &SimpleThreadWork::run, getThread(isAddedThread, lpParam), 0, NULL);
#else
  pthread_create(&threadHandle,NULL, &SimpleThreadWork::run,getThread(isAddedThread,lpParam));
#endif
}

void SimpleThread::setAliveState(const bool value)
{
  isAlive = value;
}

void SimpleThread::waitAndDeleteThreadHandle()
{
#ifdef WINDOWS_SYSTEM
  if (threadHandle)
  {
    WaitForSingleObject(threadHandle, INFINITE);
    CloseHandle(threadHandle);
    threadHandle = nullptr;
  }
#else
  pthread_join(threadHandle,NULL);
#endif
}

const volatile bool SimpleThread::getAliveState()
{
  return isAlive;
}

ThreadExtraWork* SimpleThread::getThread(const bool isAddedThread, LPVOID lpParam)
{
  return isAddedThread
      ? new ThreadExtraWorkAnyTime(poolData, this, lpParam)
      : static_cast<ThreadExtraWork *>(new ThreadExtraWorkAllTime(poolData, this, lpParam));
}


THREAD_RETURN_TYPE SimpleThreadWork::run(LPVOID lpParam)
{
  ThreadExtraWork *objectParam = static_cast<ThreadExtraWork*>(lpParam);
  THREAD_RETURN_TYPE result= objectParam->complete();
  delete objectParam;
  return result;
}
