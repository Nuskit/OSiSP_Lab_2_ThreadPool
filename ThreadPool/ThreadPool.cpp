#include "stdafx.h"
#include "ThreadPool.h"
#include "SimpleThread.h"
#include "ThreadPoolData.h"
#include "Monitor.h"
#include "Functor.h"
#include "Tasks.h"
#include "ILogger.h"
#include "MonitorRAII.h"

ThreadPool::ThreadPool(const shared_ptr<ILogger>& logger, const UINT maxCountPool, const UINT countPool):
	simpleThreads(countPool), poolData(new ThreadPoolData()), logger(logger)
{
	_minCountPool = countPool;
	_maxCountPool = maxCountPool < countPool ? countPool : maxCountPool;
	createPool();
}

ThreadPool::~ThreadPool()
{
	deletePool();
	delete poolData;
}

VOID ThreadPool::addTask(const shared_ptr<ThreadDelegateFunctor>& task)
{
	if (!poolData->getCountWorkTask())
		synchronizePool();
	if (poolData->getCountWorkTask() == _maxCountPool)
		printf("Max num task\n");//log max workThread
	else
	{
		if (poolData->getCountWorkTask() >= _minCountPool)
			generateThread();
		runTask(task);
	}
}

void ThreadPool::createPool()
{
	for (UINT i = 0; i < _minCountPool; ++i)
	{
		simpleThreads[i] = new SimpleThread(poolData);
		simpleThreads[i]->run(NULL);
	}
}

VOID ThreadPool::deletePool()
{
	for (UINT i = 0; i < simpleThreads.size(); ++i)
		simpleThreads[i]->setAliveState();
	
	synchronizePool();
	poolData->getThreadMonitor().PulseAll();

	for (UINT i = 0; i < simpleThreads.size(); ++i)
		delete simpleThreads[i];
	simpleThreads.clear();
}

//we need check, that thread in wait or other state
void ThreadPool::synchronizePool()
{
	MonitorRAII threadMonitor(&poolData->getThreadMonitor());
}

void ThreadPool::generateThread()
{
	if (simpleThreads.size() - poolData->getCountDeleteThread() <= poolData->getCountWorkTask())
	{
		SimpleThread* currentThread = (poolData->getCountDeleteThread() > 0) ? getOldThread() : generateNewThread();
		currentThread->run(NULL, true);
	}
}

SimpleThread* ThreadPool::getOldThread()
{
	auto thread = poolData->getFirstDeleteThread();
	auto threadIt=std::find(simpleThreads.begin(), simpleThreads.end(), thread);
	if (threadIt !=simpleThreads.end())
			(*threadIt)->waitAndDeleteThreadHandle();
	return (*threadIt);
}

SimpleThread * ThreadPool::generateNewThread()
{
	auto newThread = new SimpleThread(poolData);
	simpleThreads.push_back(newThread);
	return newThread;
}

void ThreadPool::runTask(const shared_ptr<ThreadDelegateFunctor>& task)
{
	poolData->getTask().addTask(task);
	poolData->getThreadMonitor().Pulse();
	poolData->incCountWorkTask();
}
