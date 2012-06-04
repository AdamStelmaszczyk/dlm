/**
 * try_lock_test.cpp
 *
 * @class lock_test
 * @brief Small program testing try_lock function
 * @date 03-06-2012
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
	int sleep_duration = atoi(argv[3]);

	DLM_init(argc, argv);
	DLM_trylock(resource_id, (LockType) lock_type);
	sleep(sleep_duration);
	return 0;
}



