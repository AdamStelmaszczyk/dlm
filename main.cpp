#include <iostream>

#include "config/Config.h"
#include "ui/SimpleConsole.h"
#include "lock_manager/LockManager.h"

using namespace std;
using namespace dlm;

int main()
{
	Config config("config.cfg");

	LockManager lm;

	SimpleConsole console(cin, cout, config, lm);
	console.start();

	return 0;
}
