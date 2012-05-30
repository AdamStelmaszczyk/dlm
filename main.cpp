#include <iostream>
#include <signal.h>
#include <pthread.h>
#include "config/Config.h"
#include "ui/SimpleConsole.h"
#include "lock_manager/LockManager.h"

#include "api/dlm.h"

using namespace std;
using namespace dlm;

int main(int argc, char **argv)
{
	Config config("config.cfg");
	LockManager lm;

	// Block SIGCHLD signals from children, cleaner will handle them.
	sigset_t sigs_to_block;
	sigemptyset(&sigs_to_block);
	sigaddset(&sigs_to_block, SIGCHLD);
	pthread_sigmask(SIG_BLOCK, &sigs_to_block, NULL);

	SimpleConsole console(cin, cout, config, lm);
	console.start();

	return 0;
}
