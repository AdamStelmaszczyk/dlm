/**
 * lock_test.cpp
 *
 * @class lock_test
 * @brief Small program testing lock function
 * @date 02-06-2012
 */

#include <unistd.h>
#include <cstdlib>
#include <iostream>

#include "../api/dlm.h"

using namespace std;

int main(int argc, char **argv)
{
	//Przygotowanie argumentów.
	int resource_id = atoi(argv[1]);
	int lock_type = atoi(argv[2]);
	long timeout = atol(argv[3]);
	int sleep_duration = atoi(argv[4]);

	DLM_init(argc, argv);
	DLM_lock(resource_id, (LockType)lock_type, timeout);
	sleep(sleep_duration);
	return 0;
}
