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
	 * Logs plain message
	 *
	 * @param message
	 */
	void log(const std::string& message);
	/**
	 * Logs message connected with execution of a function. Includes file name, function name and line number
	 * Use __FILE__, __FUNCTION__, __LINE__ to obtain necessary values
	 *
	 * @param filename
	 * @param function
	 * @param line_number
	 * @param message
	 */
	void log(const char* filename, const char* function, size_t line_number, const std::string& message);
private:
	Logger(){}
	Logger(const Logger&);
	/** output stream */
	std::ostream* output_stream;
};
}

#endif /* LOGGER_H_ */
