/**
 * test.cpp
 *
 * @class test
 * @brief Small program testing DLM API.
 * @date 21-05-2012
 */

#include <unistd.h>
#include <cstdlib>
#include <iostream>

#include "../api/dlm.h"

using namespace std;

int main(int argc, char **argv)
{
	DLM_init(argc, argv);
	DLM_lock(1, EX, 0);
	sleep(5);
	return 0;
}
