#ifndef MUTEXRAII_H_
#define MUTEXRAII_H_
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
#endif
