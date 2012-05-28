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

void Logger::log(const std::string& message)
{
	*output_stream << "[" << message  << "]" << std::endl;
}

}
