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
	cout << "Start of test process" << endl;
	DLM_init(argc, argv);
	sleep(1);
	DLM_lock(1, CR, 5000);
	sleep(1);
	DLM_unlock(1);
	sleep(3);
}
