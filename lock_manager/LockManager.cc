/**
 * LockManager.cc
 *
 * @class LockManager
 * @date 20-05-2012
 */

#include "LockManager.h"
#include <iostream>

namespace dlm
{

LockManager::LockManager()
{

}

int LockManager::lock(LockRequest r, pid_t pid)
{
	std::cout << "proces " << pid << "wykonuje locka na zasob "
			  << r.resource_ << std::endl;
}

int LockManager::unlock(UnlockRequest r, pid_t pid)
{
}

int LockManager::tryLock(TryLockRequest r, pid_t pid)
{
}

LockManager::~LockManager()
{
}

} /* namespace dlm */
