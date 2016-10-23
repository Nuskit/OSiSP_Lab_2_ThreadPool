#include "stdafx.h"
#include "ConsoleLogger.h"
#include <iostream>
#include "Functor.h"
using namespace std;


void ConsoleLogger::createThreadPool(UINT count)
{
	cout << "Create ThreadPool at " << count << " count." << endl;
}

void ConsoleLogger::addTaskPool(const std::shared_ptr<ThreadDelegateFunctor>& task)
{
	cout << "Add new task " << task << "- ";
	task->toString(cout);
	cout << endl;
}

void ConsoleLogger::errorMaxTask()
{
	cout << "Max complete task" << endl;
}

void ConsoleLogger::errorWorkPool(const std::shared_ptr<ThreadDelegateFunctor>& task, const std::exception* error)
{
	cout << "Error complete -";
	task->toString(cout);
	if (error)
		cout << " Because" << endl <<error->what();
	cout << endl;
}

void ConsoleLogger::errorCreateThreadPool(UINT count)
{
	cout << "Don't create pool at " << count << " count." << endl;
}
