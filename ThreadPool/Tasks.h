#pragma once
#include "stdafx.h"

class ThreadDelegateFunctor;
class Monitor;
class Tasks
{
public:
	Tasks();
	std::shared_ptr<ThreadDelegateFunctor> getTask();
	void addTask(const std::shared_ptr<ThreadDelegateFunctor>& task);
	size_t getTaskCount();
private:
	Monitor* taskMonitor;
	std::list<std::shared_ptr<ThreadDelegateFunctor>> tasks;
};

