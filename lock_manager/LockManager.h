/**
 * LockManager.h
 *
 * @class LockManager
 * @brief Stores info about locks, provides methods
 *  	  to lock and unlock resource.
 * @date 20-05-2012
 */

#ifndef LOCKMANAGER_H_
#define LOCKMANAGER_H_

#include <queue>
#include <sys/types.h>

#include "LockRequest.h"
#include "UnlockRequest.h"
#include "TryLockRequest.h"
#include "../api/dlm.h"
#include "../resource/Resource.h"

namespace dlm
{

class LockManager
{
public:
	/**
	 * Locks resource for specified process.
	 *
	 * @param lock_request Request structure.
	 * @param pid Process Identifier, PID.
	 *
	 * @return If 0 - ok, otherwise - error.
	 */
	int lock(LockRequest lock_request, pid_t pid);
	int unlock(UnlockRequest unlock_request, pid_t pid);
	int tryLock(TryLockRequest trylock_request, pid_t pid);
	LockManager();

	/**
	 * removes any information (from accepted and blocked locks queues)
	 * about that process.
	 *
	 * @param pid - pid of process
	 */
	void cleanup(pid_t pid);
	virtual ~LockManager();
};

}

#endif /* LOCKMANAGER_H_ */
