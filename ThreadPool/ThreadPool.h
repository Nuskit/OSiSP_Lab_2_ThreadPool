#pragma once
#include "stdafx.h"

using namespace std;

class Monitor;
class ThreadPool
{
	ThreadPool(const UINT maxCountPool,const UINT countPool);
	~ThreadPool();

	VOID addTask(LPVOID task);
private:
	VOID createPool();
	VOID deletePool();
	
	DWORD WINAPI simpleThread(LPVOID lpParam);

	UINT _minCountPool;
	UINT _currentCountPool;
	UINT _maxCountPool;

	vector<HANDLE> threadPool;
	Monitor *monitor;
};
