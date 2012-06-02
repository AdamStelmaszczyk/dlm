#include "Error.h"

using namespace dlm;
using namespace std;

Error::Error(const string &msg, const string &file, int line_no, int err_num) :
		Warning(msg, file, line_no, err_num)
{
	class_name = "Error";
}

Error::~Error() throw ()
{
}
