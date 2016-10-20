#include "stdafx.h"
#include "ThreadExtraWork.h"
#include "Monitor.h"
#include "ThreadPoolData.h"
#include "SimpleThread.h"
#include "Tasks.h"
#include "Functor.h"
#include "MonitorRAII.h"

#define TIMEOUT 1000

ThreadInfo::ThreadInfo(ThreadPoolData* poolData, SimpleThread* simpleThread, LPVOID lpParam) :poolData(poolData), simpleThread(simpleThread), lpParam(lpParam)
{
}

ThreadExtraWork::ThreadExtraWork(ThreadPoolData * poolData, SimpleThread* simpleThread, LPVOID lpParam) :threadInfo(poolData, simpleThread, lpParam)
{
}

DWORD ThreadExtraWork::complete()
{
	MonitorRAII threadMonitor(&threadInfo.poolData->getThreadMonitor());
	releaseMainThread();
	printf("Enter current thread %d\n", GetCurrentThreadId());
	while (threadInfo.simpleThread->getAliveState())
	{
		printf("Start wait_ %d\n", GetCurrentThreadId());
		waitTask();
		printf("End wait_ %d\n", GetCurrentThreadId());
		tryCompleteTask();
	}
	printf("Exit current thread %d\n", GetCurrentThreadId());
	notifyExit();
	return EXIT_SUCCESS;
}

void ThreadExtraWork::releaseMainThread()
{
	MonitorRAII mainThreadMonitor(&threadInfo.poolData->getSyncStartThreadMonitor());
	mainThreadMonitor.pulse();
}

ThreadExtraWork::~ThreadExtraWork()
{
}

void ThreadExtraWork::tryCompleteTask()
{
	if (threadInfo.simpleThread->getAliveState())
	{
		auto task = threadInfo.poolData->getTask().getTask();
		try
		{
			task->complete();
		}
		catch (std::exception* e)
		{
			//logging error
			printf("%s\n", e->what());
		}
		threadInfo.poolData->decCountWorkTask();
	}
}

ThreadExtraWorkAllTime::ThreadExtraWorkAllTime(ThreadPoolData * poolData, SimpleThread* simpleThread, LPVOID lpParam):ThreadExtraWork(poolData, simpleThread, lpParam)
{
}

void ThreadExtraWorkAllTime::waitTask()
{
	threadInfo.poolData->getThreadMonitor().Wait();
}

void ThreadExtraWorkAllTime::notifyExit()
{
}

ThreadExtraWorkAnyTime::ThreadExtraWorkAnyTime(ThreadPoolData * poolData, SimpleThread* simpleThread, LPVOID lpParam):ThreadExtraWork(poolData, simpleThread, lpParam)
{
}

void ThreadExtraWorkAnyTime::waitTask()
{
	threadInfo.simpleThread->setAliveState(threadInfo.simpleThread->getAliveState() & threadInfo.poolData->getThreadMonitor().Wait(TIMEOUT));//check what return command
}

void ThreadExtraWorkAnyTime::notifyExit()
{
	threadInfo.poolData->addDeleteThread(const_cast<const SimpleThread *&>(threadInfo.simpleThread));
}