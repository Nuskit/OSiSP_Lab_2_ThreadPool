#pragma once
#include "stdafx.h"

class ThreadPoolData;
class ThreadExtraWork;
class SimpleThread
{
public:
	SimpleThread(ThreadPoolData *poolData);
	~SimpleThread();
	void run(LPVOID lpParam, const bool isAddedThread = false);
	void setAliveState(const bool value=false);
	void waitAndDeleteThreadHandle();
	const bool getAliveState();
private:
	ThreadExtraWork* getThread(const bool isAddedThread,LPVOID lpParam);

	ThreadPoolData *poolData;
	volatile bool isAlive;
	HANDLE threadHandle;
};

class SimpleThreadWork
{
public:
	static DWORD WINAPI run(LPVOID lpParam);
private:
	SimpleThreadWork();
};