#ifndef TASKS_H_
#define TASKS_H_
#include "stdafx.h"

class ThreadDelegateFunctor;
class Mutex;
class Tasks
{
public:
	Tasks();
	~Tasks();
	const std::shared_ptr<ThreadDelegateFunctor> getTask();
	void addTask(const std::shared_ptr<ThreadDelegateFunctor>& task);
	size_t getTaskCount();
private:
	Mutex* taskMutex;
	std::queue<std::shared_ptr<ThreadDelegateFunctor>> tasks;
};
#endif
