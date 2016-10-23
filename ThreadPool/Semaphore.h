#pragma once

#include "stdafx.h"

#define MAX_COUNT_SEMAPHORE 1000

class Semaphore
{
public:
	Semaphore();
	~Semaphore();

	void wait();
	const bool wait(const DWORD milliSeconds);
	void pulse();
	void pulseAll();
private:
	HANDLE semaphore;
};