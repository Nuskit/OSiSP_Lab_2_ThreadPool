#pragma once

#include "stdafx.h"
#include "BaseLogger.h"

class ConsoleLogger :public BaseLogger
{
	virtual std::ostream & getStream() override;
};