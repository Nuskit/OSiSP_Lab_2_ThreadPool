#include "stdafx.h"
#include "ThreadExtraWork.h"
#include "Semaphore.h"
#include "ThreadPoolData.h"
#include "SimpleThread.h"
#include "Tasks.h"
#include "Functor.h"
#include "ILogger.h"

#define WAIT_TIMEOUT_TIME 1000

ThreadInfo::ThreadInfo(ThreadPoolData* poolData, SimpleThread* simpleThread, LPVOID lpParam) :poolData(poolData), simpleThread(simpleThread), lpParam(lpParam)
{
}

ThreadExtraWork::ThreadExtraWork(ThreadPoolData * poolData, SimpleThread* simpleThread, LPVOID lpParam) :threadInfo(poolData, simpleThread, lpParam)
{
}

DWORD ThreadExtraWork::complete()
{
	while (threadInfo.simpleThread->getAliveState())
	{
		waitTask();
		tryCompleteTask();
	}
	notifyExit();
	return EXIT_SUCCESS;
}

ThreadExtraWork::~ThreadExtraWork()
{
}

void ThreadExtraWork::tryCompleteTask()
{
	if (threadInfo.simpleThread->getAliveState())
	{
		auto task = threadInfo.poolData->getTasks().getTask();
		try
		{
			task->complete();
		}
		catch (const std::exception& e)
		{

			threadInfo.poolData->getLogger()->errorWorkPool(task, &e);
		}
		catch (...)
		{
			threadInfo.poolData->getLogger()->errorWorkPool(task);
		}
		threadInfo.poolData->decCountWorkTask();
	}
}

ThreadExtraWorkAllTime::ThreadExtraWorkAllTime(ThreadPoolData * poolData, SimpleThread* simpleThread, LPVOID lpParam):ThreadExtraWork(poolData, simpleThread, lpParam)
{
}

void ThreadExtraWorkAllTime::waitTask()
{
	threadInfo.poolData->getThreadSemaphore().wait();
}

void ThreadExtraWorkAllTime::notifyExit()
{
}

ThreadExtraWorkAnyTime::ThreadExtraWorkAnyTime(ThreadPoolData * poolData, SimpleThread* simpleThread, LPVOID lpParam):ThreadExtraWork(poolData, simpleThread, lpParam)
{
}

void ThreadExtraWorkAnyTime::waitTask()
{
	threadInfo.simpleThread->setAliveState(threadInfo.simpleThread->getAliveState() & threadInfo.poolData->getThreadSemaphore().wait(WAIT_TIMEOUT_TIME));
}

void ThreadExtraWorkAnyTime::notifyExit()
{
	threadInfo.poolData->addDeleteThread(const_cast<const SimpleThread *&>(threadInfo.simpleThread));
}