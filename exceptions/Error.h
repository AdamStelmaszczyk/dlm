/**
 * Error.h
 *
 * @class Error
 * @brief Indicates app error (should terminate app).
 * @date 21-05-2012
 */

#ifndef ERROR_H_
#define ERROR_H_

#include <exception>
#include <errno.h>
#include <cstdio>

#define ERROR(a) Error((a), __FILE__, __LINE__)
#define ERROR2(a, errno_) Error((a), __FILE__, __LINE__, (errno_))

namespace dlm
{
class Error: public std::exception
{
public:
	Error(const std::string &msg, const std::string &file, unsigned line_no, int err_num = 0) :
			msg_(msg), file_(file), lineNumber_(line_no), errno_(err_num)
	{
	}

	virtual ~Error() throw ()
	{
	}

	virtual const char* what() const throw ()
	{
		char line_no[256], err[256]; // line number; errno
		sprintf(line_no, "%d", lineNumber_);
		if(errno != 0) // errno == 0 means: no error
			sprintf(err, "%d", errno_);
		std::string res("");
		res += std::string("ERROR - FILE ")
				+ file_
				+ std::string(", ")
				+ std::string(line_no)
				+ std::string(" : ")
				+ msg_
				+ (errno != 0 ? std::string(" ERRNO: ") + std::string(err) : std::string(""))
				;
		return res.c_str();
	}
private:
	/** errno value **/
	int errno_;
	/** warning message */
	std::string msg_;
	/** file where message was called */
	std::string file_;
	/** line no. where message was called */
	unsigned lineNumber_;
};

} /* namespace dlm */
#endif /* ERROR_H_ */
