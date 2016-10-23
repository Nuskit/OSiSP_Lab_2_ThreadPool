#include "stdafx.h"
#include "FileLogger.h"
using namespace std;

FileLogger::FileLogger(const char * fileName):stream(fileName,std::ofstream::app)
{
}

FileLogger::~FileLogger()
{
	stream.close();
}

ostream& FileLogger::getStream()
{
	return stream;
}
