/**
 * Warning.h
 *
 * @class Warning
 * @brief Indicates app warning (needn't terminate app).
 * @date 20-05-2012
 */

#ifndef WARNING_H_
#define WARNING_H_

#include <exception>
#include <cstdio>
#include <errno.h>
#include <string>

// Macros to call warning with selected file and line number.
#define WARNING(a) Warning((a), __FILE__, __LINE__)
#define WARNING2(a, errno) Warning((a), __FILE__, __LINE__, (errno))

namespace dlm
{

class Warning: public std::exception
{
public:
	Warning(const std::string &msg, const std::string &file, int line_no, int err_num = 0);
	virtual ~Warning() throw ();
	virtual const char* what(char *message) const throw ();

protected:
	/** Error number. */
	int error_number;
	/** Warning message. */
	std::string msg;
	/** File where message was called. */
	std::string file;
	/** Line no. where message was called. */
	int line_number;
	/** Name of this class. */
	std::string class_name;
};

}

#endif /* WARNING_H_ */
