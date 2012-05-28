/**
 * Warning.h
 *
 * @class Warning
 * @brief Indicates app warning (needn't terminate app).
 * @date 20-05-2012
 */

#ifndef PIPEERROREXCEPTION_H_
#define PIPEERROREXCEPTION_H_
#include <exception>
#include <cstdlib>
#include <cstdio>
#include <errno.h>
#include <sstream>

// macro to call warning with selected file and line number
#define WARNING(a) Warning((a), __FILE__, __LINE__)
#define WARNING2(a, errno) Warning((a), __FILE__, __LINE__, (errno))

namespace dlm
{

class Warning: public std::exception
{
public:
	Warning(const std::string &msg, const std::string &file, unsigned line_no, int err_num = 0) :
			msg_(msg), file_(file), lineNumber_(line_no), errno_(err_num)
	{
	}

	virtual ~Warning() throw ()
	{
	}

	virtual const char* what() const throw ()
	{
		char line_no[256], err[256]; // line number; errno
		sprintf(line_no, "%d", lineNumber_);
		if(errno != 0) // errno == 0 means: no error
			sprintf(err, "%d", errno_);
		//this hack resolves the problem with disappearing error messages
		std::string res = "";
		res +=  std::string("WARNING - FILE ")
				+ file_
				+ std::string(",")
				+ std::string(line_no)
				+ std::string(" : ")
				+ msg_
				+ (errno != 0 ? std::string(" ERRNO: ") + std::string(err) : std::string(""))
				;
		return res.c_str();
	}
private:
	/** error number */
	int errno_;
	/** warning message */
	std::string msg_;
	/** file where message was called */
	std::string file_;
	/** line no. where message was called */
	unsigned lineNumber_;
};

} /* namespace dlm */
#endif /* PIPEERROREXCEPTION_H_ */
