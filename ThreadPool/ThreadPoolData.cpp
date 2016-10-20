#include "stdafx.h"
#include "ThreadPoolData.h"
#include "Monitor.h"
#include "Tasks.h"
#include "MonitorRAII.h"

ThreadPoolData::ThreadPoolData() :currentWorkTask(0), tasks(new Tasks()), threadMonitor(new Monitor()),syncStartThreadMonitor(new Monitor), waitDeleteMonitor(new Monitor())
{
}

ThreadPoolData::~ThreadPoolData()
{
	delete threadMonitor;
	delete syncStartThreadMonitor;
	delete waitDeleteMonitor;
	delete tasks;
	std::queue<const SimpleThread*> empty;
	std::swap(waitDeleteThread, empty);
}

Monitor& ThreadPoolData::getThreadMonitor()
{
	return *threadMonitor;
}

Monitor& ThreadPoolData::getSyncStartThreadMonitor()
{
	return *syncStartThreadMonitor;
}

Tasks& ThreadPoolData::getTask()
{
	return *tasks;
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
	MonitorRAII waitDeleteMonitor(waitDeleteMonitor);
	auto deleteThread = waitDeleteThread.front();
	waitDeleteThread.pop();
	return deleteThread;
}

void ThreadPoolData::addDeleteThread(const SimpleThread *& thread)
{
	MonitorRAII waitDeleteMonitor(waitDeleteMonitor);
	waitDeleteThread.push(thread);
}

size_t ThreadPoolData::getCountDeleteThread()
{
	return waitDeleteThread.size();
}
