#include "ui/SimpleConsole.h"
#include <iostream>

int main()
{
	dlm::SimpleConsole console(std::cin, std::cout);
	console.start();

	return 0;
}
