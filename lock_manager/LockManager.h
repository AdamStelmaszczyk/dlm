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
	 * locks resource for specified process
	 *
	 * @param r - request structure
	 * @param process
	 *
	 * @return 0 - ok, != 0 - error
	 */
	int lock(LockRequest r, pid_t pid);
	int unlock(UnlockRequest r, pid_t pid);
	int tryLock(TryLockRequest r, pid_t pid);
	LockManager();
	virtual ~LockManager();
private:

};

} /* namespace dlm */
#endif /* LOCKMANAGER_H_ */
