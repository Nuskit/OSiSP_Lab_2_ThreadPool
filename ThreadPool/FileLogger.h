#pragma once

#include "stdafx.h"
#include "BaseLogger.h"
#include <fstream>

class FileLogger:public BaseLogger
{
public:
	FileLogger(const char* fileName="logs.txt");
	~FileLogger();
protected:
	virtual std::ostream& getStream() override;
private:
	std::ofstream stream;
};