#pragma once
#include "stdafx.h"

using namespace std;

class ThreadPool
{
	ThreadPool(const UINT countPool);
	~ThreadPool();
private:
	VOID CreatePool();
	VOID DeletePool();
	
	DWORD WINAPI SimpleThread(LPVOID lpParam);

	UINT _countPool;
	
	vector<HANDLE> threadPool;
};
