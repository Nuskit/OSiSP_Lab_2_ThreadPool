#include "stdafx.h"
#include "Monitor.h"

Monitor::Monitor()
{
	::InitializeCriticalSectionAndSpinCount(&m_mutex,4000);
	::InitializeConditionVariable(&m_condVar);
}

Monitor::~Monitor()
{
	::DeleteCriticalSection(&m_mutex);
}

void Monitor::Enter()
{
	::EnterCriticalSection(&m_mutex);
}

bool Monitor::TryEnter()
{
	return ::TryEnterCriticalSection(&m_mutex);
}

void Monitor::Exit()
{
	::LeaveCriticalSection(&m_mutex);
}

void Monitor::Wait(const DWORD milliSeconds=INFINITE)
{
	::SleepConditionVariableCS(&m_condVar, &m_mutex, milliSeconds);
}

void Monitor::Pulse()
{
	::WakeConditionVariable(&m_condVar);
}

void Monitor::PulseAll()
{
	WakeAllConditionVariable(&m_condVar);
}
