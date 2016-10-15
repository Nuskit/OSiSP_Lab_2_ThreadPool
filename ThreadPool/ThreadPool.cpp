#include "ThreadPool.h"
#include "Monitor.h"

ThreadPool::ThreadPool(const UINT maxCountPool, const UINT countPool):threadPool(countPool),monitor(new Monitor())
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
	for (int i = 0; i < _minCountPool; i++)
		threadPool[i] = CreateThread(NULL, 0, this->simpleThread, NULL, 0, NULL);;
}

//TODO: wait, why all thread exit
VOID ThreadPool::deletePool()
{
	//Wait all 

	for (UINT i = 0; i < _currentCountPool; i++)
		CloseHandle(threadPool[i]);
	threadPool.clear();
}

DWORD ThreadPool::SimpleThread(LPVOID lpParam)
{
	SleepCon
	WaitCreai
	EnterCriticalSection();
}
