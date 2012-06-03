/**
 * LockManager.cc
 *
 * @class LockManager
 * @brief Stores info about locks, provides methods to lock and unlock resource.
 * @date 01-06-2012
 */

#include <stack>
#include <errno.h>
#include <iostream>
#include <sys/time.h>

#include "Lock.h"
#include "LockOwner.h"
#include "LockManager.h"
#include "../logger/Logger.h"
#include "WaitingLock.h"

using namespace std;
using namespace dlm;

bool LockManager::permission[5][5] =
{
{ 1, 1, 1, 1, 0 },
{ 1, 1, 0, 0, 0 },
{ 1, 0, 1, 0, 0 },
{ 1, 0, 0, 0, 0 },
{ 0, 0, 0, 0, 0 }, };

int LockManager::lock(LockRequest request, pid_t pid)
{
	// Iterate through all active locks.
	for (list<Lock>::iterator it = active_locks.begin(); it != active_locks.end(); ++it)
	{
		if (request.rid != it->request.rid || permission[request.locktype][it->request.locktype])
		{
			// Not matching RID or the same RID, but we have permission for additional lock.
			continue;
		}

		if (pid == it->pid)
		{
			return ALREADY_HAVE_LOCK;
		}

		// There is a conflict with some active lock.

		if (request.timeout == -1)
		{
			// Non-block mode, we immediately return error code.
			return ALREADY_LOCKED;
		}

		pthread_cond_t cond;
		pthread_cond_init(&cond, NULL);

		pthread_mutex_t mutex;
		pthread_mutex_init(&mutex, NULL);
		pthread_mutex_lock(&mutex);

		WaitingLock waiting_lock(request, pid, &cond);
		waiting_locks.push_back(waiting_lock);

		Logger::getInstance().log("[%d is waiting for RID %d]", pid, request.rid);

		if (request.timeout == 0)
		{
			pthread_cond_wait(&cond, &mutex);
		}
		else
		{
			struct timespec timeout = get_timespec(request);
			int result = pthread_cond_timedwait(&cond, &mutex, &timeout);

			if (result == ETIMEDOUT)
			{
				// Process timed out waiting. He is no more waiting, remove him from waiting list.
				waiting_locks.remove(waiting_lock);
				Logger::getInstance().log("[processs %d timed out]", pid);
				return TIMEOUT;
			}
		}

		// If we are here - process was awaken, so it got its lock.
		Logger::getInstance().log("[awaken process %d locks with type %d on RID %d with timeout %d]",
								   pid, request.locktype, request.rid, request.timeout);
		return OK;
	}

	// If we are here - there are no conflicts with active locks, so we can set requested lock.
	Lock lock(request, pid);
	active_locks.push_back(lock);

	Logger::getInstance().log("[process %d locks with type %d on RID %d with timeout %d]",
							   pid, request.locktype, request.rid, request.timeout);
	return OK;
}

void LockManager::awake_waiting(rid_t rid)
{
	// Collect all locks that are waiting on given RID.
	stack<WaitingLock> locks_stack;
	// Going backwards, because locks from the beginning of waiting list should be on top of the stack.
	for (list<WaitingLock>::reverse_iterator it = waiting_locks.rbegin(); it != waiting_locks.rend(); ++it)
	{
		if (it->request.rid == rid)
		{
			locks_stack.push(*it);
		}
	}

	// Try to wake up locks collected in stack.
	while (!locks_stack.empty())
	{
		WaitingLock waiting_lock = locks_stack.top();
		locks_stack.pop();

		TryLockRequest tryRequest =
		{ waiting_lock.request.rid, waiting_lock.request.locktype };

		if (tryLock(tryRequest, waiting_lock.pid) == 0)
		{
			// Copy lock to active list.
			Lock lock(waiting_lock.request, waiting_lock.pid);
			active_locks.push_back(lock);

			// Remove waiting lock.
			waiting_locks.remove(waiting_lock);

			// Wake him up.
			pthread_cond_signal(waiting_lock.cond);
		}
	}
}

int LockManager::unlock(UnlockRequest request, pid_t pid)
{
	Logger::getInstance().log("[process %d unlocks RID %d]", pid, request.rid);
	for (list<Lock>::iterator it = active_locks.begin(); it != active_locks.end(); ++it)
	{
		if (request.rid == it->request.rid)
		{
			// Remove this active lock.
			active_locks.remove(*it);

			// Awake processes waiting for this RID.
			awake_waiting(request.rid);

			// One process could have only one lock on this RID, so we are done.
			return OK;
		}
	}
	return NO_SUCH_LOCK;
}

int LockManager::tryLock(TryLockRequest request, pid_t pid)
{
	Logger::getInstance().log("[process %d tries lock %d on RID %d]",
							   pid, request.locktype, request.rid);
	// Iterate through all active locks.
	for (list<Lock>::iterator it = active_locks.begin(); it != active_locks.end(); ++it)
	{
		if (request.rid == it->request.rid && !permission[request.locktype][it->request.locktype])
		{
			// There is a conflict with some active lock.
			Logger::getInstance().log("[process %d, you can't have this lock!]", pid);
			return ALREADY_LOCKED;
		}
	}
	Logger::getInstance().log("[process %d, you can have this lock]", pid);
	// There are no conflicts with active locks, so it is possible to set given lock.
	return OK;
}

void LockManager::cleanup(pid_t pid)
{
	Logger::getInstance().log("[cleanup after process %d]", pid);
	waiting_locks.remove_if(LockOwner(pid));

	// Collect all RID's that belong to given PID.
	stack<rid_t> rid_stack;
	for (list<Lock>::iterator it = active_locks.begin(); it != active_locks.end(); ++it)
	{
		if (it->pid == pid)
		{
			rid_stack.push(it->request.rid);
		}
	}

	// Unlock these RID's.
	while (!rid_stack.empty())
	{
		UnlockRequest request =
		{ rid_stack.top() };
		rid_stack.pop();
		unlock(request, pid);
	}
}

struct timespec LockManager::get_timespec(LockRequest& request)
{
	struct timeval now;
	struct timespec timeout;
	gettimeofday(&now, NULL);
	// 1 s = 10^3 milliseconds = 10^6 microseconds = 10^9 nanoseconds
	timeout.tv_sec = now.tv_sec + (request.timeout / 1000);
	timeout.tv_nsec = (1000 * now.tv_usec) + (1000000 * request.timeout);
	if (timeout.tv_nsec >= 1000000000)
	{
		timeout.tv_nsec -= 1000000000;
		timeout.tv_sec++;
	}
	return timeout;
}

LockManager::~LockManager()
{
}
