#include "stdafx.h"
#include "MutexRAII.h"
#include "Mutex.h"

MutexRAII::MutexRAII(Mutex *mutex):mutex(mutex)
{
	mutex->enter();
}

MutexRAII::~MutexRAII()
{
	mutex->exit();
}
