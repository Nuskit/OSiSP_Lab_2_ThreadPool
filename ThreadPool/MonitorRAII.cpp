#include "stdafx.h"
#include "MonitorRAII.h"
#include "Monitor.h"

MonitorRAII::MonitorRAII(Monitor * monitor):monitor(monitor)
{
	monitor->Enter();
}

void MonitorRAII::wait()
{
	monitor->Wait();
}

void MonitorRAII::pulse()
{
	monitor->Pulse();
}

MonitorRAII::~MonitorRAII()
{
	monitor->Exit();
}
