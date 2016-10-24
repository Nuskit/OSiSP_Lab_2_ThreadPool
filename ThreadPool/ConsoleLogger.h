#ifndef CONSOLELOGGER_H_
#define CONSOLELOGGER_H_

#include "stdafx.h"
#include "BaseLogger.h"

class ConsoleLogger :public BaseLogger
{
	virtual std::ostream & getStream() override;
};
#endif
