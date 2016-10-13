#include "ThreadPool.h"

ThreadPool::ThreadPool(const UINT countPool):threadPool(countPool)
{
	_countPool = countPool;
	CreatePool();
}

ThreadPool::~ThreadPool()
{
	DeletePool();
}

void ThreadPool::CreatePool()
{
	for (int i = 0; i < _countPool; i++)
		CreateThread(NULL, 0, this->SimpleThread, NULL, 0, NULL);
}

//TODO: wait, why all thread exit
VOID ThreadPool::DeletePool()
{
	threadPool.clear();
}

DWORD ThreadPool::SimpleThread(LPVOID lpParam)
{
	return 0;
}
