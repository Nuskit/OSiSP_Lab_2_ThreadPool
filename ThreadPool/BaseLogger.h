#pragma once

#include "stdafx.h"
#include "ILogger.h"
#include <ostream>

class Mutex;
class BaseLogger : public ILogger
{
public:
	BaseLogger();
	virtual ~BaseLogger();
	virtual void errorCreateThreadPool(UINT count) override;
	virtual void createThreadPool(UINT count) override;
	virtual void addTaskPool(const std::shared_ptr<ThreadDelegateFunctor>& task) override;
	virtual void errorWorkPool(const std::shared_ptr<ThreadDelegateFunctor>& task, const std::exception* error) override;
	virtual void errorMaxTask() override;
protected:
	void writeCurrentTime();
	virtual std::ostream& getStream() = 0;
private:
	Mutex* mutex;
};