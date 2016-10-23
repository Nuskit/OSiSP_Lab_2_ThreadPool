#pragma once

#include "stdafx.h"
#include "ILogger.h"

class ConsoleLogger :public ILogger
{
	virtual void errorCreateThreadPool(UINT count) override;
	virtual void createThreadPool(UINT count) override;
	virtual void addTaskPool(const std::shared_ptr<ThreadDelegateFunctor>& task) override;
	virtual void errorWorkPool(const std::shared_ptr<ThreadDelegateFunctor>& task, const std::exception* error) override;
	virtual void errorMaxTask() override;
};