#include "Logger.h"
#include <cstdio>

namespace dlm
{
Logger& Logger::getInstance()
{
	static Logger instance;
	return instance;
}

void Logger::setOutputStream(std::ostream& output)
{
	output_stream = &output;
}

void Logger::log(const char* format, ... )
{
	char buffer[1024];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(buffer, format, argptr);
    va_end(argptr);

    *output_stream << buffer;
}

}
