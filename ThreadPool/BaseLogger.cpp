#include "stdafx.h"
#include "BaseLogger.h"
#include "Functor.h"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <ios>
#include "Mutex.h"
#include "MutexRAII.h"
using namespace std;

BaseLogger::BaseLogger():mutex(new Mutex())
{
}

BaseLogger::~BaseLogger()
{
	delete mutex;
}

void BaseLogger::writeCurrentTime()
{
	auto now = std::chrono::system_clock::now();
	auto now_c = std::chrono::system_clock::to_time_t(now);
	getStream() << std::put_time(std::localtime(&now_c), "%c") << endl;
}

void BaseLogger::errorCreateThreadPool(UINT count)
{
	MutexRAII streamMutex(mutex);
	writeCurrentTime();
	getStream() << "Don't create pool at " << count << " count." << endl;
}

void BaseLogger::createThreadPool(UINT count)
{
	MutexRAII streamMutex(mutex);
	writeCurrentTime();
	getStream() << "Create ThreadPool at " << count << " count." << endl;
}

void BaseLogger::addTaskPool(const std::shared_ptr<ThreadDelegateFunctor>& task)
{
	MutexRAII streamMutex(mutex);
	writeCurrentTime();
	getStream() << "Add new task " << task << "- ";
	task->toString(getStream());
	getStream() << endl;
}

void BaseLogger::errorWorkPool(const std::shared_ptr<ThreadDelegateFunctor>& task, const std::exception * error)
{
	MutexRAII streamMutex(mutex);
	writeCurrentTime();
	getStream() << "Error complete -";
	task->toString(getStream());
	if (error)
		getStream() << " Because" << endl << error->what();
	getStream() << endl;
}

void BaseLogger::errorMaxTask()
{
	MutexRAII streamMutex(mutex);
	writeCurrentTime();
	getStream() << "Max complete task" << endl;
}