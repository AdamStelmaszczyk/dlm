#include "ui/SimpleConsole.h"
#include <iostream>

using namespace dlm;
int main()
{
	SimpleConsole console(std::cin, std::cout);
	console.start();
	return 0;
}
