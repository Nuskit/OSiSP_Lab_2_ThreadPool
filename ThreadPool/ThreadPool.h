#pragma once
#include "stdafx.h"
#include <atomic>
using namespace std;

class ThreadDelegateFunctor;
class SimpleThread;
class ThreadPoolData;
class ILogger;
class ThreadPool
{
public:
	ThreadPool(const shared_ptr<ILogger>& logger,const UINT maxCountPool,const UINT countPool);
	~ThreadPool();

	VOID addTask(const shared_ptr<ThreadDelegateFunctor>& task);
private:
	ThreadPool(const ThreadPool& object);
	ThreadPool& operator=(const ThreadPool& object);
	
	VOID createPool();
	VOID deletePool();
	void generateThread();
	void synchronizePool();
	SimpleThread* getOldThread();
	SimpleThread* generateNewThread();
	void runTask(const shared_ptr<ThreadDelegateFunctor>& task);
	UINT _minCountPool;
	UINT _maxCountPool;
	
	vector<SimpleThread*> simpleThreads;
	ThreadPoolData *poolData;
	const shared_ptr<ILogger> logger;
};