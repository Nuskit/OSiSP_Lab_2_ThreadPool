#include "stdafx.h"
#include "ThreadPool.h"
#include "Monitor.h"

#define TIMEOUT 5000

ThreadPool::ThreadPool(const UINT maxCountPool, const UINT countPool)
	:threadPool(countPool),simpleThreads(countPool), monitor(new Monitor())
{
	_minCountPool = _currentCountPool = countPool;
	_maxCountPool = maxCountPool < countPool ? countPool : maxCountPool;
	createPool();
}

ThreadPool::~ThreadPool()
{
	deletePool();
	delete monitor;
}

void ThreadPool::createPool()
{
	for (UINT i = 0; i < _minCountPool; i++)
	{
		simpleThreads[i] = new SimpleThread(monitor);
		threadPool[i] = simpleThreads[i]->run(NULL);
	}
}

//TODO: wait, why all thread exit
VOID ThreadPool::deletePool()
{
	//Wait all 
	for (UINT i = 0; i < _currentCountPool; ++i)
		simpleThreads[i]->setDiedState();
	monitor->PulseAll();

	WaitForMultipleObjects(_currentCountPool, &threadPool[0], true, INFINITE);
	for (UINT i = 0; i < _currentCountPool; ++i)
		CloseHandle(threadPool[i]);
	threadPool.clear();
}

ThreadPool::SimpleThread::SimpleThread(Monitor* monitor):monitor(monitor),isAlive(false)
{
}

const HANDLE& ThreadPool::SimpleThread::run(LPVOID lpParam,const bool isAddedThread)
{
	isAlive = true;
	return 0;
	//return CreateThread(NULL, 0, &(this->thread), lpParam, 0, NULL);
}

void ThreadPool::SimpleThread::setDiedState()
{
	isAlive = false;
}

DWORD ThreadPool::SimpleThread::thread(LPVOID lpParam)
{
	do
	{
		monitor->Wait();
	} while (isAlive);
	return EXIT_SUCCESS;
}

DWORD ThreadPool::SimpleThread::addedThread(LPVOID lpParam)
{
	do
	{
		isAlive &= monitor->Wait(TIMEOUT);//check what return command
	} while (isAlive);
	return EXIT_SUCCESS;
}

void ThreadPool::SimpleThread::tryCompleteTask()
{
	if (isAlive)
	{
		try
		{
			monitor->Enter();
		}
		catch(exception)
		{
			monitor->Exit();
			throw;
		}
		monitor->Exit();
	}
}

LPTHREAD_START_ROUTINE ThreadPool::SimpleThread::getThread(const bool isAddedThread)
{
	return NULL;
//	return &this->addedThread;
	//return isAddedThread ? this->addedThread : &this->thread;
}