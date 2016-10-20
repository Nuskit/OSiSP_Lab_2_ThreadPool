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
	Monitor& getThreadMonitor();
	Monitor& getSyncStartThreadMonitor();
	Tasks& getTask();
	void incCountWorkTask();
	void decCountWorkTask();
	const UINT getCountWorkTask();
	const SimpleThread* getFirstDeleteThread();
	void addDeleteThread(const SimpleThread*& thread);
	size_t getCountDeleteThread();
private:
	std::atomic<UINT> currentWorkTask;
  Tasks* tasks;
	Monitor* threadMonitor;
	Monitor* syncStartThreadMonitor;
	Monitor* waitDeleteMonitor;
	std::queue<const SimpleThread*> waitDeleteThread;
};
