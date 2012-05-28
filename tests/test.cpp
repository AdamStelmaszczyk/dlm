/**
 * test.cpp
 *
 * @class test
 * @brief Brief description.
 * @date 21-05-2012
 */
#include <unistd.h>
#include <cstdlib>
#include "../api/dlm.h"

int main(int argc, char **argv)
{
	DLM_init(argc, argv);
	DLM_lock(1, LockType::CR, 1000);
}

