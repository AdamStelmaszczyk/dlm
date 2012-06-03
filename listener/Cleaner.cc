/**
 * Cleaner.cc
 *
 * @class Cleaner
 * @brief Cleans after child processes. Stops listener thread and calls for cleaning in LockManager.
 * @date 29-05-2012
 */

#include <sys/types.h>
#include <iostream>
#include <sys/wait.h>
#include <signal.h>

#include "Cleaner.h"
#include "../logger/Logger.h"

using namespace std;
using namespace dlm;

Cleaner::Cleaner(LockManager &lm) :
		lock_manager(lm)
{
	pthread_mutex_init(&procs_mutex, NULL);
}

void Cleaner::start()
{
	siginfo_t info;
	sigset_t sigs_to_catch;
	sigemptyset(&sigs_to_catch);
	sigaddset(&sigs_to_catch, SIGCHLD);
	for (;;)
	{
		sigwaitinfo(&sigs_to_catch, &info);
		int si_code = info.si_code;
		if (si_code == CLD_EXITED || si_code == CLD_KILLED || si_code == CLD_DUMPED)
		{
			// Omit trapped and stopped child processes.
			pid_t pid = wait(NULL); // Get PID of terminated process.
			removeClient(pid);
		}
	}
}

void Cleaner::addClient(pid_t process, pthread_t listener)
{
	pthread_mutex_lock(&procs_mutex);
	// Critical section.
	procs.insert(make_pair(process, listener));
	pthread_mutex_unlock(&procs_mutex);
}

void Cleaner::removeClient(pid_t process)
{
	pthread_mutex_lock(&procs_mutex);
	map<pid_t, pthread_t>::iterator it = procs.find(process); // Finds listener thread.
	if (it != procs.end())
	{
		pthread_t t = it->second;
		pthread_cancel(t); // Stop listeners thread.
		lock_manager.cleanup(process);
	}
	pthread_mutex_unlock(&procs_mutex);
}

Cleaner::~Cleaner()
{
}

void* start_cleaner(void *ptr)
{
	// Start new thread.
	sigset_t sigs_to_block;
	sigemptyset(&sigs_to_block);
	pthread_sigmask(SIG_BLOCK, &sigs_to_block, NULL); // Unblock all signals.
	Cleaner *cleaner = (Cleaner*) ptr;
	cleaner->start();
	return NULL;
}
