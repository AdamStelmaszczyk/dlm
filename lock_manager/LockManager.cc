/**
 * LockManager.cc
 *
 * @class LockManager
 * @date 30-05-2012
 */

#include <errno.h>
#include <iostream>
#include <sys/time.h>

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

	// Iterate through all active locks.
	for (list<Lock>::iterator it = active_locks.begin(); it != active_locks.end(); ++it)
	{
		if (request.rid == it->request.rid && !permission[request.locktype][it->request.locktype])
		{
			// There is a conflict with some active lock.
			waiting_locks.push_back(lock);

			if (it->request.timeout == -1)
			{
				// Non-block mode, we immediately return error code.
				return -1; // TODO: stałe na errory
			}

			pthread_cond_t cond;
			pthread_cond_init(&cond, NULL);

			pthread_mutex_t mutex;
			pthread_mutex_init(&mutex, NULL);
			pthread_mutex_lock(&mutex);

			struct timeval now;
			struct timespec timeout;
			gettimeofday(&now, NULL);
			timeout.tv_sec = now.tv_sec + request.timeout / 1000;
			timeout.tv_nsec = now.tv_usec * 1000 + request.timeout * 1000000;

			int result = pthread_cond_timedwait(&cond, &mutex, &timeout);
			if (result == ETIMEDOUT)
			{
				// Process timed out waiting.
				return -1; // TODO: stałe na errory
			}

			// If we are here - process was awaken, so it got its lock.
			return 0;
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
	// Iterate through all active locks.
	for (list<Lock>::iterator it = active_locks.begin(); it != active_locks.end(); ++it)
	{
		if (request.rid == it->request.rid && !permission[request.locktype][it->request.locktype])
		{
			// There is a conflict with some active lock.
			return -1;
		}
	}
	// There are no conflicts with active locks, so it is possible to set given lock.
	return 0;
}

void LockManager::cleanup(pid_t pid)
{
	cout << "cleanup after process " << pid << endl;
	waiting_locks.remove_if(LockOwner(pid));
	for (list<Lock>::iterator it = active_locks.begin(); it != active_locks.end(); ++it)
	{
		if (it->pid == pid)
		{
			UnlockRequest request = { it->request.rid };
			unlock(request, pid);
		}
	}
}

LockManager::~LockManager()
{
}
