#ifndef THREADPOOL_H_
#define THREADPOOL_H_
#include "stdafx.h"
#include <atomic>
using namespace std;

#define DEFAULT_WORK_THREAD_POOL 4

class ThreadDelegateFunctor;
class SimpleThread;
class ThreadPoolData;
class ILogger;
class ThreadPool
{
public:
	ThreadPool(const shared_ptr<ILogger>& logger,const UINT countPool= DEFAULT_WORK_THREAD_POOL, const UINT maxCountPool = DEFAULT_WORK_THREAD_POOL);
	~ThreadPool();

	void addTask(const shared_ptr<ThreadDelegateFunctor>& task);
	const bool havePool();
private:
	ThreadPool(const ThreadPool& object);
	ThreadPool& operator=(const ThreadPool& object);

	void setMaxCountPoolNotLessThanCountPool(const UINT countPool,UINT maxCountPool);
	void createPool();
	void deletePool();
	void addNewThread();
	SimpleThread* getDeleteThread();
	SimpleThread* createNewThread();
	SimpleThread* generateThread();
	const SimpleThread* getFirstDeleteThread();
	const bool isWorkTaskMoreThanAliveThread();
	const bool isMaxCountWorkTask();
	const bool isCountWorkTaskMoreThanMinimalCountPool();
	void deleteSimpleThread();
	void setSimpleThreadTerminatedCondition();
	void runTask(const shared_ptr<ThreadDelegateFunctor>& task);
	
	UINT minCountPool_;
	UINT maxCountPool_;
	vector<SimpleThread*> simpleThreads;
	ThreadPoolData *poolData;
	bool isCreatePool;
};
#endif
