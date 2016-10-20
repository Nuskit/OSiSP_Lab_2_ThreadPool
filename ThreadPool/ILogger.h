#pragma once

#include "stdafx.h"

class ThreadDelegateFunctor;
class ILogger
{
public:
	virtual void createThreadPool(UINT count) = 0;
	virtual void addTaskPool(std::shared_ptr<ThreadDelegateFunctor>& task) = 0;
	virtual void errorWorkPool(std::exception& error) = 0;
	virtual void warningWorkPool(std::string message) = 0;
};
