#pragma once

#include "stdafx.h"

class Monitor
{
public:
	Monitor();
	~Monitor();

	void Enter();
	bool TryEnter();
	void Exit();
	void Wait(const DWORD);
	void Pulse();
	void PulseAll();
private:
	CRITICAL_SECTION m_mutex;
	CONDITION_VARIABLE m_condVar;
};