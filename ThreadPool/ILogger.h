#pragma once

#include "stdafx.h"

class ThreadDelegateFunctor;
class ILogger
{
public:
	virtual void errorCreateThreadPool(UINT count) = 0;
	virtual void createThreadPool(UINT count) = 0;
	virtual void addTaskPool(const std::shared_ptr<ThreadDelegateFunctor>& task) = 0;
	virtual void errorWorkPool(const std::shared_ptr<ThreadDelegateFunctor>& task, const std::exception* error = nullptr) = 0;
	virtual void errorMaxTask() = 0;
};
