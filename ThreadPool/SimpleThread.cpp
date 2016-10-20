#include "stdafx.h"
#include "SimpleThread.h"
#include "ThreadPoolData.h"
#include "Monitor.h"
#include "ThreadExtraWork.h"

SimpleThread::SimpleThread(ThreadPoolData * poolData) :poolData(poolData), isAlive(false), threadHandle(nullptr)
{
}

SimpleThread::~SimpleThread()
{
	waitAndDeleteThreadHandle();
}

void SimpleThread::run(LPVOID lpParam, const bool isAddedThread)
{
	isAlive = true;
	poolData->incCountWorkPool();
	threadHandle = CreateThread(NULL, 0, &SimpleThreadWork::run, getThread(isAddedThread, lpParam), 0, NULL);
}

void SimpleThread::setAliveState(const bool value)
{
	isAlive = value;
}

void SimpleThread::waitAndDeleteThreadHandle()
{
	if (threadHandle)
	{
		WaitForSingleObject(threadHandle, INFINITE);
		CloseHandle(threadHandle);
		threadHandle = nullptr;
	}
}

const bool SimpleThread::getAliveState()
{
	return isAlive;
}

ThreadExtraWork* SimpleThread::getThread(const bool isAddedThread, LPVOID lpParam)
{
	return isAddedThread
		? new ThreadExtraWorkAnyTime(poolData, this, lpParam)
		: static_cast<ThreadExtraWork *>(new ThreadExtraWorkAllTime(poolData, this, lpParam));
}


DWORD SimpleThreadWork::run(LPVOID lpParam)
{
	ThreadExtraWork *objectParam = static_cast<ThreadExtraWork*>(lpParam);
	DWORD result= objectParam->complete();
	delete objectParam;
	return result;
}