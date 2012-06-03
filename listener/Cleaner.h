/**
 * Cleaner.h
 *
 * @class Cleaner
 * @brief This is only place, where signals are handled. Cleaner waits for some signals and cleans after
 * 	      listeners (child processes). Start should be called in separate thread.
 * @date 02-06-2012
 */

#ifndef CLEANER_H_
#define CLEANER_H_

#include <map>
#include <pthread.h>

#include "../lock_manager/LockManager.h"

namespace dlm
{

class Cleaner
{
public:
	Cleaner(LockManager &lm);

	void start();

	/**
	 * Ads new client which will be handled.
	 *
	 * @param process
	 * @param listener
	 */
	void addClient(pid_t process, pthread_t listener);
	void removeClient(pid_t process);

	virtual ~Cleaner();
private:
	/** Processes and their	handlers. */
	std::map<pid_t, pthread_t> procs;
	/** Mutex to lock modifications on procs. */
	pthread_mutex_t procs_mutex;
	/** Reference to lock manager to call cleanup. */
	LockManager &lock_manager;

};

}

void *start_cleaner(void *p);

#endif /* CLEANER_H_ */
