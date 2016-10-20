#include "stdafx.h"
#include "Tasks.h"
#include "Functor.h"
#include "Monitor.h"

Tasks::Tasks() :taskMonitor(new Monitor())
{
}

Tasks::~Tasks()
{
	tasks.clear();
	delete taskMonitor;
}

const std::shared_ptr<ThreadDelegateFunctor> Tasks::getTask()
{
	taskMonitor->Enter();
	auto task = tasks.front();
	tasks.pop_front();
	taskMonitor->Exit();
	return task;
}

void Tasks::addTask(const std::shared_ptr<ThreadDelegateFunctor>& task)
{
	taskMonitor->Enter();
	tasks.push_back(task);
	taskMonitor->Exit();
}

size_t Tasks::getTaskCount()
{
	return tasks.size();
}