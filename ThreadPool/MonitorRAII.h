#pragma once
#include "stdafx.h"

class Monitor;
class MonitorRAII
{
public:
	MonitorRAII(Monitor* monitor);
	void wait();
	void pulse();
	~MonitorRAII();
private:
	Monitor* monitor;
};
