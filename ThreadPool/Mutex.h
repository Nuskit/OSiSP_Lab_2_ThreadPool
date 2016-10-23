#pragma once
#include "stdafx.h"

class Mutex
{
public:
	Mutex();
	~Mutex();
	void enter();
	const bool tryEnter();
	void exit();
private:
	CRITICAL_SECTION mutex;
};
