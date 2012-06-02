#include "Warning.h"

using namespace dlm;
using namespace std;

Warning::Warning(const string &msg, const string &file, int line_no, int err_num) :
		error_number(err_num), msg(msg), file(file), line_number(line_no), class_name("Warning")
{
}

Warning::~Warning() throw ()
{
}

const char* Warning::what(char *message) const throw ()
{
	sprintf(message, "%s - file %s , line %d : %s errno: %d", class_name.c_str(), file.c_str(), line_number, msg.c_str(), errno);
	return message;
}
