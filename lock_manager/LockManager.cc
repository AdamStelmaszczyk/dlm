/**
 * LockManager.cc
 *
 * @class LockManager
 * @date 30-05-2012
 */

#include <iostream>

#include "Lock.h"
#include "LockOwner.h"
#include "LockManager.h"

using namespace std;
using namespace dlm;

bool LockManager::permission[5][5] =
{
	{ 1, 1, 1, 1, 0 },
	{ 1, 1, 0, 0, 0 },
	{ 1, 0, 1, 0, 0 },
	{ 1, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 },
};

LockManager::LockManager()
{

}

int LockManager::lock(LockRequest request, pid_t pid)
{
	cout << "proces " << pid
			<< " wykonuje locka na zasob " << request.rid
			<< " rodzaj locka: " << request.locktype
			<< " timeout " << request.timeout
			<< endl;

	Lock lock = { request, pid };

	for (list<Lock>::iterator it = active_locks.begin(); it != active_locks.end(); it++)
	{
		if (request.rid == it->request.rid && !permission[request.locktype][it->request.locktype])
		{
			// pthread_cond_timedwait, wrzuć do waiting_locks, return
		}
	}

	// If we are here - there are no conflicts with active locks, so we can set requested lock.
	active_locks.push_back(lock);

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
	cout << "cleanup after process " << pid << endl;
	active_locks.remove_if(LockOwner(pid));
	waiting_locks.remove_if(LockOwner(pid));
}

LockManager::~LockManager()
{
}
