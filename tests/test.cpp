/**
 * test.cpp
 *
 * @class test
 * @brief Brief description.
 * @date 21-05-2012
 */
#include <unistd.h>
#include <cstdlib>
#include <iostream>
#include "../api/dlm.h"

int main(int argc, char **argv)
{
	std::cout << "proces potomny" << std::endl;
	DLM_init(argc, argv);
	DLM_lock(1, CR, 1000);
}

