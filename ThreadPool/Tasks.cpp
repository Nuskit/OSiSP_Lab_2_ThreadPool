#include "stdafx.h"
#include "Tasks.h"
#include "Functor.h"
#include "Mutex.h"
#include "MutexRAII.h"

Tasks::Tasks() :taskMutex(new Mutex())
{
}

Tasks::~Tasks()
{
	std::queue<std::shared_ptr<ThreadDelegateFunctor>> empty;
	std::swap(tasks, empty);
	delete taskMutex;
}

const std::shared_ptr<ThreadDelegateFunctor> Tasks::getTask()
{
  MutexRAII taskWorkMutex(taskMutex);
	auto task = tasks.front();
	tasks.pop();
	return task;
}

void Tasks::addTask(const std::shared_ptr<ThreadDelegateFunctor>& task)
{
  MutexRAII taskWorkMutex(taskMutex);
	tasks.push(task);
}

size_t Tasks::getTaskCount()
{
	return tasks.size();
}
