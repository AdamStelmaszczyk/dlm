#include "ui/SimpleConsole.h"
#include "config/Config.h"
#include "lock_manager/LockManager.h"
#include <iostream>

int main()
{
	dlm::Config config(dlm::Config("config.cfg"));
	config.readInput();

	dlm::LockManager lm;

	dlm::SimpleConsole console(std::cin, std::cout, config, lm);
	console.start();

	return 0;
}
