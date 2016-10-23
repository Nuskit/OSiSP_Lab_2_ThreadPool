#include "stdafx.h"
#include "ThreadPoolData.h"
#include "Semaphore.h"
#include "Mutex.h"
#include "MutexRAII.h"
#include "Tasks.h"

ThreadPoolData::ThreadPoolData(const std::shared_ptr<ILogger>& logger) 
	: currentWorkTask(0), tasks(new Tasks()), threadSemaphore(new Semaphore()), waitDeleteMutex(new Mutex()),logger_(logger)
{
}

ThreadPoolData::~ThreadPoolData()
{
	delete threadSemaphore;
	delete waitDeleteMutex;
	delete tasks;
	std::queue<const SimpleThread*> empty;
	std::swap(waitDeleteThread, empty);
}

Semaphore& ThreadPoolData::getThreadSemaphore()
{
	return *threadSemaphore;
}

Tasks& ThreadPoolData::getTasks()
{
	return *tasks;
}

std::shared_ptr<ILogger> & ThreadPoolData::getLogger()
{
	return logger_;
}

void ThreadPoolData::incCountWorkTask()
{
	++currentWorkTask;
}

void ThreadPoolData::decCountWorkTask()
{
	--currentWorkTask;
}

const UINT ThreadPoolData::getCountWorkTask()
{
	return currentWorkTask;
}

const SimpleThread* ThreadPoolData::getFirstDeleteThread()
{
	MutexRAII waitMutex(waitDeleteMutex);
	auto deleteThread = waitDeleteThread.front();
	waitDeleteThread.pop();
	return deleteThread;
}

void ThreadPoolData::addDeleteThread(const SimpleThread *& thread)
{
	MutexRAII waitMutex(waitDeleteMutex);
	waitDeleteThread.push(thread);
}

size_t ThreadPoolData::getCountDeleteThread()
{
	return waitDeleteThread.size();
}
