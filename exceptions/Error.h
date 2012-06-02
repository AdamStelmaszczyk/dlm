/**
 * Error.h
 *
 * @class Error
 * @brief Indicates app error (should terminate app).
 * @date 21-05-2012
 */

#ifndef ERROR_H_
#define ERROR_H_

#include "Warning.h"

#define ERROR(a) Error((a), __FILE__, __LINE__)
#define ERROR2(a, errno_) Error((a), __FILE__, __LINE__, (errno_))

namespace dlm
{

class Error: public Warning
{
public:
	Error(const std::string &msg, const std::string &file, int line_no, int err_num = 0);
	virtual ~Error() throw ();
};

}

#endif /* ERROR_H_ */
