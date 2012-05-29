/**
 * LockManager.cc
 *
 * @class LockManager
 * @date 20-05-2012
 */

#include "LockManager.h"
#include <iostream>

using namespace std;

namespace dlm
{

LockManager::LockManager()
{

}

int LockManager::lock(LockRequest r, pid_t pid)
{
	cout << "proces " << pid << " wykonuje locka na zasob " << r.resource_ << " rodzaj locka: " << r.locktype_ << " timeout " << r.timeout_
			<< endl;
	return 0;
}

int LockManager::unlock(UnlockRequest r, pid_t pid)
{
	return 0;
}

int LockManager::tryLock(TryLockRequest r, pid_t pid)
{
	return 0;
}

void LockManager::cleanup(pid_t pid)
{
}

LockManager::~LockManager()
{
}

} /* namespace dlm */
