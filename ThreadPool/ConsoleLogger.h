#pragma once

#include "stdafx.h"
#include "ILogger.h"

class ConsoleLogger :public ILogger
{
	virtual void createThreadPool(UINT count) override;
	virtual void addTaskPool(std::shared_ptr<ThreadDelegateFunctor>& task) override;
	virtual void errorWorkPool(std::exception & error) override;
	virtual void warningWorkPool(std::string message) override;
};