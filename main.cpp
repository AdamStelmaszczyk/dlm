#include <iostream>
#include <signal.h>
#include <pthread.h>

#include "api/dlm.h"
#include "config/Config.h"
#include "ui/SimpleConsole.h"
#include "lock_manager/LockManager.h"

using namespace std;
using namespace dlm;

//ifstream *input_file = NULL;
//ofstream *output_file = NULL;
//
//void parse_args(int argc, char *argv[])
//{
//}

int main(int argc, char *argv[])
{

	Config config("config.cfg");
	LockManager lm;

	// Block SIGCHLD signals from children, cleaner will handle them.
	signal(SIGPIPE, SIG_IGN); // It's easier to handle corrupted pipe with write/read return code.
	sigset_t sigs_to_block;
	sigemptyset(&sigs_to_block);
	sigaddset(&sigs_to_block, SIGCHLD);
	sigaddset(&sigs_to_block, SIGPIPE);
	pthread_sigmask(SIG_BLOCK, &sigs_to_block, NULL);

	SimpleConsole console(cin, cout, config, lm);
	int result = console.start();
//	delete input_file;
//	delete output_file;
	return result;
}
