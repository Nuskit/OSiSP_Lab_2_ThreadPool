#pragma once

#include "stdafx.h"

class Mutex;
class MutexRAII
{
public:
	MutexRAII(Mutex*);
	~MutexRAII();
private:
	Mutex *mutex;
};
