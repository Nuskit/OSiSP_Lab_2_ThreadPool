#ifndef MUTEX_H_
#define MUTEX_H_
#include "stdafx.h"

#ifdef WINDOWS_SYSTEM
  #define SYNC_PRIMITIVE_MUTEX CRITICAL_SECTION
#else
  #define SYNC_PRIMITIVE_MUTEX pthread_mutex_t
#endif

class Mutex
{
public:
	Mutex();
	~Mutex();
	void enter();
  const bool tryEnter();
	void exit();
private:
  SYNC_PRIMITIVE_MUTEX mutex;
};
#endif
