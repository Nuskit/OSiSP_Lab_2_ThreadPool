#pragma once
#include "stdafx.h"


class SimpleThread;
class ThreadExtraWork;
class ThreadPoolData;
struct ThreadInfo
{
	ThreadInfo(ThreadPoolData* poolData, SimpleThread* simpleThread, LPVOID lpParam);
	ThreadPoolData* poolData;
	SimpleThread *simpleThread;
	LPVOID lpParam;
};

class ThreadExtraWork
{
public:
	ThreadExtraWork(ThreadPoolData* poolData, SimpleThread* simpleThread, LPVOID lpParam);
	DWORD complete();
	virtual ~ThreadExtraWork() {};
private:
	virtual void waitTask() = 0;
	void tryCompleteTask();
	virtual void notifyExit() = 0;
protected:
	ThreadInfo threadInfo;
};

class ThreadExtraWorkAllTime :public ThreadExtraWork
{
public:
	ThreadExtraWorkAllTime(ThreadPoolData* poolData, SimpleThread* simpleThread, LPVOID lpParam);
	virtual void waitTask() override;
	virtual void notifyExit() override;
};

class ThreadExtraWorkAnyTime :public ThreadExtraWork
{
public:
	ThreadExtraWorkAnyTime(ThreadPoolData* poolData, SimpleThread* simpleThread, LPVOID lpParam);
	virtual void waitTask() override;
	virtual void notifyExit() override;
};