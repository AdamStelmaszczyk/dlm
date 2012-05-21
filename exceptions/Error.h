/**
 * Error.h
 *
 * @class Error
 * @brief Indicates app error (should terminate app).
 * @author nieznany
 * @date 21-05-2012
 */

#ifndef ERROR_H_
#define ERROR_H_

#include <exception>
#include <cstdio>

#define ERROR(a) Error((a), __FILE__, __LINE__)

namespace dlm
{
class Error: public std::exception
{
public:
	Error(const std::string &msg, const std::string &file, unsigned line_no) :
			msg_(msg), file_(file), lineNumber_(line_no)
	{
	}

	virtual ~Error() throw ()
	{
	}

	virtual const char* what() const throw ()
	{
		char line_no[256];
		sprintf(line_no, "%d", lineNumber_);
		return (std::string("FILE ") + file_ + std::string(",") +
						std::string(line_no) + std::string(" : ") + msg_).c_str();
	}
private:
	/** warning message */
	std::string msg_;
	/** file where message was called */
	std::string file_;
	/** line no. where message was called */
	unsigned lineNumber_;
};

} /* namespace dlm */
#endif /* ERROR_H_ */
