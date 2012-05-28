#include "ui/SimpleConsole.h"
#include "config/Config.h"
#include <iostream>

int main()
{
	dlm::Config config(dlm::Config("config.cfg"));
	config.readInput();

	dlm::SimpleConsole console(std::cin, std::cout, config);
	console.start();

	return 0;
}
