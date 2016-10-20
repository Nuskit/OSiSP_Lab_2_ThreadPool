#include "stdafx.h"
#include "ConsoleLogger.h"
#include <iostream>
using namespace std;


void ConsoleLogger::createThreadPool(UINT count)
{
	cout << "Create ThreadPool at " << count << " count." << endl;
}

void ConsoleLogger::addTaskPool(std::shared_ptr<ThreadDelegateFunctor>& task)
{
	cout << "Add new task " << task << endl;
}

void ConsoleLogger::errorWorkPool(std::exception & error)
{
	cout << "Error complete " << error.what() << endl;
}

void ConsoleLogger::warningWorkPool(std::string message)
{
	cout << "Warning, " << message << endl;
}
