#include "stdafx.h"
#include "ThreadPoolData.h"
#include "Monitor.h"
#include "Tasks.h"

ThreadPoolData::ThreadPoolData() :currentWorkPool(0), tasks(new Tasks()), monitor(new Monitor()), waitDeleteMonitor(new Monitor())
{
}

ThreadPoolData::~ThreadPoolData()
{
	delete monitor;
	delete waitDeleteMonitor;
}

Monitor& ThreadPoolData::getMonitor()
{
	return *monitor;
}

Tasks& ThreadPoolData::getTask()
{
	return *tasks;
}

void ThreadPoolData::incCountWorkPool()
{
	++currentWorkPool;
}

void ThreadPoolData::decCountWorkPool()
{
	--currentWorkPool;
}

const UINT ThreadPoolData::getCountWorkPool()
{
	return currentWorkPool;
}

const SimpleThread* ThreadPoolData::getFirstDeleteThread()
{
	waitDeleteMonitor->Enter();
	auto thread=waitDeleteThread.front();
	waitDeleteMonitor->Exit();
	return thread;
}

void ThreadPoolData::addDeleteThread(const SimpleThread * thread)
{
	waitDeleteMonitor->Enter();
	waitDeleteThread.push_back(thread);
	waitDeleteMonitor->Exit();
}

size_t ThreadPoolData::getCountDeleteThread()
{
	return waitDeleteThread.size();
}
