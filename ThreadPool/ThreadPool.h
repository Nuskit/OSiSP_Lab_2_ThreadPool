#pragma once
#include "stdafx.h"
#include "Functor.h"
using namespace std;

class Monitor;
class ThreadPool
{
public:
	ThreadPool(const UINT maxCountPool,const UINT countPool);
	~ThreadPool();

	VOID addTask(LPVOID task);
private:
	ThreadPool(const ThreadPool& object);
	ThreadPool& operator=(const ThreadPool& object);

	class SimpleThread
	{
	public:
		SimpleThread(Monitor*);
		const HANDLE& run(LPVOID lpParam,const bool isAddedThread=false);
		void setDiedState();
	private:
		DWORD WINAPI thread(LPVOID lpParam);
		DWORD WINAPI addedThread(LPVOID lpParam);
		void tryCompleteTask();

		LPTHREAD_START_ROUTINE getThread(const bool isAddedThread);

		Monitor *monitor;
		volatile bool isAlive;
	};

	/*class Tasks
	{
	public:
		DelegateFunctor<void> getTask();
	private:
		list<DelegateFunctorImpl<> tasks;
	};*/

	VOID createPool();
	VOID deletePool();
	
	UINT _minCountPool;
	UINT _currentCountPool;
	UINT _maxCountPool;
	
	vector<HANDLE> threadPool;
	vector<SimpleThread*> simpleThreads;

	Monitor* monitor;//change to unique_ptr
};