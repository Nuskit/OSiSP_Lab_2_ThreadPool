#include "stdafx.h"
#include "Mutex.h"

#define SPIN_COUNT 4000

Mutex::Mutex()
{
#ifdef WINDOWS_SYSTEM
	::InitializeCriticalSectionAndSpinCount(&mutex, SPIN_COUNT);
#else
  pthread_mutexattr_t attr;
  pthread_mutexattr_init(&attr);
  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&mutex,&attr);
#endif
}

Mutex::~Mutex()
{
#ifdef WINDOWS_SYSTEM
	::DeleteCriticalSection(&mutex);
#else
  pthread_mutex_destroy(&mutex);
#endif
}

void Mutex::enter()
{
#ifdef WINDOWS_SYSTEM
	::EnterCriticalSection(&mutex);
#else
  pthread_mutex_lock(&mutex);
#endif
}

const bool Mutex::tryEnter()
{
#ifdef WINDOWS_SYSTEM
  return ::TryEnterCriticalSection(&mutex)!=0;
#else
  return pthread_mutex_trylock(&mutex)==0;
#endif
}

void Mutex::exit()
{
#ifdef WINDOWS_SYSTEM
	::LeaveCriticalSection(&mutex);
#else
  pthread_mutex_unlock(&mutex);
#endif
}
