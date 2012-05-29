/**
 * Cleaner.cc
 *
 * @class Cleaner
 * @brief Cleans after child processes - stops listnener thread and cleans
 * locks in dlm.
 * @date 29-05-2012
 */

#include "Cleaner.h"
#include <sys/types.h>
#include <iostream>
#include <sys/wait.h>
#include <signal.h>

using namespace std;

namespace dlm
{

Cleaner::Cleaner(LockManager &lm):
	lm_(lm)
{
	pthread_mutex_init(&procs_mutex_, NULL);
}

void Cleaner::start()
{
	siginfo_t info;
	sigset_t sigs_to_catch;
	sigemptyset(&sigs_to_catch);
	sigaddset(&sigs_to_catch, SIGCHLD);
	// sigaddset(&sigs_to_catch, SIGPIPE); only SIGCHLD ( TODO how to get pid from SIGPIPE?)
	for (;;)
	{
		sigwaitinfo(&sigs_to_catch, &info);
		int si_code = info.si_code;
		if(si_code == CLD_EXITED || si_code == CLD_KILLED || si_code == CLD_DUMPED)
		{ // we ommit trapped and stopped child processes
			pid_t pid = wait(NULL); // get pid of terminated process
			std::cout << "proces " << pid << " sie skonczyl" << std::endl;
			removeClient(pid);
		}
	}
}

void Cleaner::addClient(pid_t process, pthread_t listener)
{
	pthread_mutex_lock(&procs_mutex_);
	// critical section
	procs_.insert(make_pair(process, listener));
	pthread_mutex_unlock(&procs_mutex_);
}

void Cleaner::removeClient(pid_t process)
{
	pthread_mutex_lock(&procs_mutex_);
	map<pid_t, pthread_t>::iterator it = procs_.find(process); // finds listener thread FIXME quite danger, I don't check if process realy exist in map
	if(it != procs_.end())
	{
		pthread_t t = it->second;
		pthread_cancel(t); // stop listeners thread
		lm_.cleanup(process);
	}
	pthread_mutex_unlock(&procs_mutex_);
}

Cleaner::~Cleaner()
{
}

void *start_cleaner(void *ptr)
{
	// here starts new thread
	Cleaner *c = (Cleaner*) ((ptr));
	c->start();
	return NULL;
}

/* namespace dlm */
}

