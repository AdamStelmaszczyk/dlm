/**
 * LockManager.h
 *
 * @class LockManager
 * @brief Stores info about locks, provides methods to lock and unlock resource.
 * @date 30-05-2012
 */

#ifndef LOCKMANAGER_H_
#define LOCKMANAGER_H_

#include <map>
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
	int lock(LockRequest request, pid_t pid);
	int unlock(UnlockRequest request, pid_t pid);
	int tryLock(TryLockRequest request, pid_t pid);
	void cleanup(pid_t pid);
	rid_t create_file_resource(const char* path);
	virtual ~LockManager();

private:
	void awakeWaiting(rid_t rid);
	struct timespec getTimespec(LockRequest& request);

	std::list<Lock> active_locks;
	std::list<WaitingLock> waiting_locks;
	std::map<const char*, rid_t> resources;
	static bool permission[5][5];
};

}

#endif /* LOCKMANAGER_H_ */
