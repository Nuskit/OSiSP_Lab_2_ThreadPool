#include "stdafx.h"
#include "ThreadPool.h"
#include "SimpleThread.h"
#include "ThreadPoolData.h"
#include "Monitor.h"
#include "Functor.h"
#include "Tasks.h"
#include "ILogger.h"

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
	synchronizePool();
	if (poolData->getCountWorkPool() == _maxCountPool)
		printf("Max num task\n");//log max workThread
	else
	{
		if (poolData->getCountWorkPool() >= _minCountPool)
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

//TODO: wait, why all thread exit
VOID ThreadPool::deletePool()
{
	//Wait all 
	for (UINT i = 0; i < simpleThreads.size(); ++i)
		simpleThreads[i]->setAliveState();
	
	synchronizePool();
	poolData->getMonitor().PulseAll();

	for (UINT i = 0; i < simpleThreads.size(); ++i)
		delete simpleThreads[i];
	simpleThreads.clear();
}

//we need check, that thread in wait or other state
void ThreadPool::synchronizePool()
{
	poolData->getMonitor().Enter();
	poolData->getMonitor().Exit();
}

void ThreadPool::generateThread()
{
	SimpleThread* currentThread = (poolData->getCountDeleteThread() > 0) ? getOldThread() : generateNewThread();
	currentThread->run(NULL, true);
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
	poolData->getMonitor().Pulse();
	poolData->incCountWorkPool();
}
