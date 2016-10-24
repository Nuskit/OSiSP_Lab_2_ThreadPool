#ifndef THREADPOOLDATA_H_
#define THREADPOOLDATA_H_
#include "stdafx.h"
#include <atomic>

class Semaphore;
class Mutex;
class Tasks;
class SimpleThread;
class ILogger;
class ThreadPoolData
{
public:
	ThreadPoolData(const std::shared_ptr<ILogger>& logger);
	~ThreadPoolData();
	Semaphore& getThreadSemaphore();
	Tasks& getTasks();
	std::shared_ptr<ILogger>& getLogger();
	void incCountWorkTask();
	void decCountWorkTask();
	const UINT getCountWorkTask();
	const SimpleThread* getFirstDeleteThread();
	void addDeleteThread(const SimpleThread*& thread);
	size_t getCountDeleteThread();
private:
	std::atomic<UINT> currentWorkTask;
  Tasks* tasks;
	Semaphore* threadSemaphore;
	Mutex* waitDeleteMutex;
	std::queue<const SimpleThread*> waitDeleteThread;
	std::shared_ptr<ILogger> logger_;
};
#endif
