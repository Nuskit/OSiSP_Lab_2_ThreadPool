#include "stdafx.h"
#include "Mutex.h"

#define SPIN_COUNT 4000

Mutex::Mutex()
{
	::InitializeCriticalSectionAndSpinCount(&mutex, SPIN_COUNT);
}

Mutex::~Mutex()
{
	::DeleteCriticalSection(&mutex);
}

void Mutex::enter()
{
	::EnterCriticalSection(&mutex);
}

const bool Mutex::tryEnter()
{
	return ::TryEnterCriticalSection(&mutex) ? true : false;
}

void Mutex::exit()
{
	::LeaveCriticalSection(&mutex);
}