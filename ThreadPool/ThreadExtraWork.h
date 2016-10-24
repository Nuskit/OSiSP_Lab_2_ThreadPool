#ifndef THREADEXTRAWORK_H_
#define THREADEXTRAWORK_H_
#include "stdafx.h"

class SimpleThread;
class ThreadExtraWork;
class ThreadPoolData;
struct ThreadInfo
{
	ThreadInfo(ThreadPoolData* poolData, SimpleThread* simpleThread, LPVOID lpParam);
	ThreadPoolData* poolData;
	SimpleThread* simpleThread;
	LPVOID lpParam;
};

class ThreadExtraWork
{
public:
	ThreadExtraWork(ThreadPoolData* poolData, SimpleThread* simpleThread, LPVOID lpParam);
  THREAD_RETURN_TYPE complete();
	virtual ~ThreadExtraWork();
private:
	virtual void waitTask() = 0;
	void tryCompleteTask();
	virtual void notifyExit() = 0;
protected:
	ThreadInfo threadInfo;
	bool isAlive;
private:
	const bool isCanCompleteWork();
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
#endif
