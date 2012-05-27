#include "Logger.h"

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

void Logger::log(const char* filename, const char* function, size_t line_number, const std::string& message)
{
	*output_stream << "[" << filename << "] " << "[" << function << "] " << "[" << line_number << "] " << message << std::endl;
}

void Logger::log(const std::string& message)
{
	*output_stream << "[" << message  << "]" << std::endl;
}

}
