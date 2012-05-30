/**
 * LockManager.cc
 *
 * @class LockManager
 * @date 30-05-2012
 */

#include "LockManager.h"
#include <iostream>

using namespace std;

namespace dlm
{

LockManager::LockManager()
{

}

int LockManager::lock(LockRequest request, pid_t pid)
{
	cout << "proces " << pid
			<< " wykonuje locka na zasob " << request.resource_
			<< " rodzaj locka: " << request.locktype_
			<< " timeout " << request.timeout_
			<< endl;
	return 0;
}

int LockManager::unlock(UnlockRequest request, pid_t pid)
{
	return 0;
}

int LockManager::tryLock(TryLockRequest request, pid_t pid)
{
	return 0;
}

void LockManager::cleanup(pid_t pid)
{
}

LockManager::~LockManager()
{
}

}
