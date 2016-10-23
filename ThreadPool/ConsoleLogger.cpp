#include "stdafx.h"
#include "ConsoleLogger.h"
#include <iostream>
#include "Functor.h"

std::ostream & ConsoleLogger::getStream()
{
	return std::cout;
}
