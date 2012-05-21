/**
 * test.cpp
 *
 * @class test
 * @brief Brief description.
 * @author nieznany
 * @date 21-05-2012
 */
#include <unistd.h>
#include <cstdlib>

int main(int argc, char **argv) {
	int req = atoi(argv[1]);
	while(1)
	{
		write(req, "tra", 3);
	}
}



