/**
 * LockManager.h
 *
 * @class LockManager
 * @brief Stores info about locks, provides methods to lock and unlock resource.
 * @date 30-05-2012
 */

#ifndef LOCKMANAGER_H_
#define LOCKMANAGER_H_

#include <list>
#include <queue>
#include <sys/types.h>

#include "Lock.h"
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
	int lock(LockRequest request, pid_t pid);
	int unlock(UnlockRequest request, pid_t pid);
	int tryLock(TryLockRequest request, pid_t pid);
	LockManager();
	/**
	 * Removes any information (from accepted and blocked locks queues) about that process.
	 *
	 * @param pid Process Identifier, PID.
	 */
	void cleanup(pid_t pid);
	virtual ~LockManager();
private:
	std::list<Lock> active_locks;
	std::queue<Lock> waiting_locks;
	static bool permission[5][5];
};

}

#endif /* LOCKMANAGER_H_ */
