#include "stdafx.h"
#include "Tasks.h"
#include "Functor.h"
#include "Monitor.h"
#include "MonitorRAII.h"

Tasks::Tasks() :taskMonitor(new Monitor())
{
}

Tasks::~Tasks()
{
	std::queue<std::shared_ptr<ThreadDelegateFunctor>> empty;
	std::swap(tasks, empty);
	delete taskMonitor;
}

const std::shared_ptr<ThreadDelegateFunctor> Tasks::getTask()
{
	MonitorRAII taskMonitor(taskMonitor);
	auto task = tasks.front();
	tasks.pop();
	return task;
}

void Tasks::addTask(const std::shared_ptr<ThreadDelegateFunctor>& task)
{
	MonitorRAII taskMonitor(taskMonitor);
	tasks.push(task);
}

size_t Tasks::getTaskCount()
{
	return tasks.size();
}