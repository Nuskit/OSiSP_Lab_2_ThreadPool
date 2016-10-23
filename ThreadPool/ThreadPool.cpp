#include "stdafx.h"
#include "ThreadPool.h"
#include "SimpleThread.h"
#include "ThreadPoolData.h"
#include "Semaphore.h"
#include "Functor.h"
#include "Tasks.h"
#include "ILogger.h"

ThreadPool::ThreadPool(const shared_ptr<ILogger>& logger, const UINT countPool, const UINT maxCountPool) :
	simpleThreads(countPool), poolData(new ThreadPoolData(logger)), isCreatePool(false)
{
	setMaxCountPoolNotLessThanCountPool(countPool, maxCountPool);
	if (maxCountPool_ <= MAX_COUNT_SEMAPHORE)
	{
		minCountPool_ = countPool;
		createPool();
		isCreatePool = true;
	}
	else
	{
		poolData->getLogger()->errorCreateThreadPool(maxCountPool_);
	}
}

void ThreadPool::setMaxCountPoolNotLessThanCountPool(const UINT countPool,UINT maxCountPool)
{
	maxCountPool_ = maxCountPool < countPool ? countPool : maxCountPool;
}

ThreadPool::~ThreadPool()
{
	if (havePool())
		deletePool();
	delete poolData;
}

void ThreadPool::addTask(const shared_ptr<ThreadDelegateFunctor>& task)
{
	if (havePool())
	{
		if (!isMaxCountWorkTask())
		{
			if (isCountWorkTaskMoreThanMinimalCountPool())
				addNewThread();
			runTask(task);
		}
		else
			poolData->getLogger()->errorMaxTask();
	}
}

const bool ThreadPool::isMaxCountWorkTask()
{
	return poolData->getCountWorkTask() == maxCountPool_;
}

const bool ThreadPool::isCountWorkTaskMoreThanMinimalCountPool()
{
	return poolData->getCountWorkTask() >= minCountPool_;
}

const bool ThreadPool::havePool()
{
	return isCreatePool;
}

void ThreadPool::createPool()
{
	for (UINT i = 0; i < minCountPool_; ++i)
	{
		simpleThreads[i] = generateThread();
		simpleThreads[i]->run(NULL);
	}
	poolData->getLogger()->createThreadPool(minCountPool_);
}

void ThreadPool::deletePool()
{
	setSimpleThreadTerminatedCondition();
	deleteSimpleThread();
}

void ThreadPool::setSimpleThreadTerminatedCondition()
{
	for (UINT i = 0; i < simpleThreads.size(); ++i)
		simpleThreads[i]->setAliveState(false);
	poolData->getThreadSemaphore().pulseAll();
}

void ThreadPool::deleteSimpleThread()
{
	for (UINT i = 0; i < simpleThreads.size(); ++i)
		delete simpleThreads[i];
	simpleThreads.clear();
}

void ThreadPool::addNewThread()
{
	if (isWorkTaskMoreThanAliveThread())
	{
		SimpleThread* currentThread = (poolData->getCountDeleteThread() > 0) ? getDeleteThread() : createNewThread();
		currentThread->run(NULL, true);
	}
}

const bool ThreadPool::isWorkTaskMoreThanAliveThread()
{
	return simpleThreads.size() - poolData->getCountDeleteThread() <= poolData->getCountWorkTask();
}

const SimpleThread* ThreadPool::getFirstDeleteThread()
{
	return poolData->getFirstDeleteThread();
}

SimpleThread* ThreadPool::getDeleteThread()
{
	auto threadIt = std::find(simpleThreads.begin(), simpleThreads.end(), getFirstDeleteThread());
	if (threadIt != simpleThreads.end())
		(*threadIt)->waitAndDeleteThreadHandle();
	return (*threadIt);
}

SimpleThread * ThreadPool::createNewThread()
{
	auto newThread = generateThread();
	simpleThreads.push_back(newThread);
	return newThread;
}

SimpleThread * ThreadPool::generateThread()
{
	return new SimpleThread(poolData);
}

void ThreadPool::runTask(const shared_ptr<ThreadDelegateFunctor>& task)
{
	poolData->getTasks().addTask(task);
	poolData->getThreadSemaphore().pulse();
	poolData->incCountWorkTask();
	poolData->getLogger()->addTaskPool(task);
}
