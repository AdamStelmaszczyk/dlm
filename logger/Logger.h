/**
 * Logger.h
 * @class Logger
 * @brief Provides simple logging facility using std::ostream output. Class implements singleton pattern.
 * @date 26-05-2012
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include <ostream>
#include <iostream>
#include <string>
#include <cstdarg>

namespace dlm
{
class Logger
{
public:
	/**
	 * Returns an instance of a logger class
	 */
	static Logger& getInstance();
	/**
	 * Set custom output stream
	 *
	 * @param output
	 */
	void setOutputStream(std::ostream& output);
	/**
	 * Logs message, uses variable argument list to provide flexible logging
	 *
	 */
	void log(const char* format, ...);
private:
	Logger(){}
	Logger(const Logger&);
	/** output stream */
	std::ostream* output_stream;
};
}

#endif /* LOGGER_H_ */
