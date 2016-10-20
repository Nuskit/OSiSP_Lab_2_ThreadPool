#pragma once
#include "stdafx.h"
#include <atomic>

class Monitor;
class Tasks;
class SimpleThread;
class ThreadPoolData
{
public:
	ThreadPoolData();
	~ThreadPoolData();
	Monitor& getMonitor();
	Tasks& getTask();
	void incCountWorkPool();
	void decCountWorkPool();
	const UINT getCountWorkPool();
	const SimpleThread* getFirstDeleteThread();
	void addDeleteThread(const SimpleThread* thread);
	size_t getCountDeleteThread();
private:
	std::atomic<UINT> currentWorkPool;
  Tasks* tasks;
	Monitor* monitor;
	Monitor* waitDeleteMonitor;
	std::list<const SimpleThread*> waitDeleteThread;
};
