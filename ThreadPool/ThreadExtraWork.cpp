#include "stdafx.h"
#include "ThreadExtraWork.h"
#include "Monitor.h"
#include "ThreadPoolData.h"
#include "SimpleThread.h"
#include "Tasks.h"
#include "Functor.h"

#define TIMEOUT 5000

ThreadInfo::ThreadInfo(ThreadPoolData* poolData, SimpleThread* simpleThread, LPVOID lpParam) :poolData(poolData), simpleThread(simpleThread), lpParam(lpParam)
{
}

ThreadExtraWork::ThreadExtraWork(ThreadPoolData * poolData, SimpleThread* simpleThread, LPVOID lpParam) :threadInfo(poolData, simpleThread, lpParam)
{
}

DWORD ThreadExtraWork::complete()
{
	threadInfo.poolData->getMonitor().Enter();
	printf("Enter current thread %d\n", GetCurrentThreadId());
	while (threadInfo.simpleThread->getAliveState())
	{
		printf("Start wait current thread %d\n", GetCurrentThreadId());
		threadInfo.poolData->decCountWorkPool();
		waitTask();
		threadInfo.poolData->incCountWorkPool();
		printf("End wait current thread %d\n", GetCurrentThreadId());
		tryCompleteTask();
	}
	threadInfo.poolData->getMonitor().Exit();
	printf("Exit current thread %d\n", GetCurrentThreadId());
	threadInfo.poolData->decCountWorkPool();
	return EXIT_SUCCESS;
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
	}
}

ThreadExtraWorkAllTime::ThreadExtraWorkAllTime(ThreadPoolData * poolData, SimpleThread * simpleThread, LPVOID lpParam):ThreadExtraWork(poolData, simpleThread, lpParam)
{
}

void ThreadExtraWorkAllTime::waitTask()
{
	threadInfo.poolData->getMonitor().Wait();
}

void ThreadExtraWorkAllTime::notifyExit()
{
}

ThreadExtraWorkAnyTime::ThreadExtraWorkAnyTime(ThreadPoolData * poolData, SimpleThread * simpleThread, LPVOID lpParam):ThreadExtraWork(poolData, simpleThread, lpParam)
{
}

void ThreadExtraWorkAnyTime::waitTask()
{
	threadInfo.simpleThread->setAliveState(threadInfo.simpleThread->getAliveState() & threadInfo.poolData->getMonitor().Wait(TIMEOUT));//check what return command
}

void ThreadExtraWorkAnyTime::notifyExit()
{
	threadInfo.poolData->addDeleteThread(threadInfo.simpleThread);
}
