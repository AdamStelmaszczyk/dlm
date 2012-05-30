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
#include <sys/types.h>

#include "Lock.h"
#include "WaitingLock.h"
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
	 * @param request Request structure.
	 * @param pid Process Identifier, PID.
	 *
	 * @return If 0 - ok, otherwise - error.
	 */
	int lock(LockRequest request, pid_t pid);

	/**
	 * Unlocks resource for specified process.
	 *
	 * @param request Request structure.
	 * @param pid Process Identifier, PID.
	 *
	 * @return If 0 - ok, otherwise - error.
	 */
	int unlock(UnlockRequest request, pid_t pid);

	/**
	 * Gives info whether is it possible to lock given resource or not. Doesn't lock anything.
	 *
	 * @param request Request structure.
	 * @param pid Process Identifier, PID.
	 *
	 * @return If 0 - yes, otherwise - no.
	 */
	int tryLock(TryLockRequest request, pid_t pid);

	/**
	 * Removes any information (from accepted and blocked locks queues) about that process.
	 *
	 * @param pid Process Identifier, PID.
	 */
	void cleanup(pid_t pid);

	virtual ~LockManager();

private:
	void awakeWaiting(rid_t rid);

	std::list<Lock> active_locks;
	std::list<WaitingLock> waiting_locks;
	static bool permission[5][5];
};

}

#endif /* LOCKMANAGER_H_ */
