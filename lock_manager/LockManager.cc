/**
 * LockManager.cc
 *
 * @class LockManager
 * @date 30-05-2012
 */

#include <stack>
#include <errno.h>
#include <iostream>
#include <sys/time.h>

#include "Lock.h"
#include "WaitingLock.h"
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
			return -2; // TODO: stałe na errory, ten proces juz ma locka na ten zasób
		}

		// There is a conflict with some active lock.

		if (it->request.timeout == -1)
		{
			// Non-block mode, we immediately return error code.
			return -1; // TODO: stałe na errory
		}

		pthread_cond_t cond;
		pthread_cond_init(&cond, NULL);

		WaitingLock waiting_lock(request, pid, cond);
		waiting_locks.push_back(waiting_lock);

		pthread_mutex_t mutex;
		pthread_mutex_init(&mutex, NULL);
		pthread_mutex_lock(&mutex);

		struct timeval now;
		struct timespec timeout;
		gettimeofday(&now, NULL);
		timeout.tv_sec = now.tv_sec + request.timeout / 1000;
		timeout.tv_nsec = now.tv_usec * 1000 + request.timeout * 1000000;

		cout << pid << " czeka" << endl;
		int result = pthread_cond_wait(&cond, &mutex); // TODO: dodać timeout
		cout << pid << " skończył czekać" << endl;

		if (result == ETIMEDOUT)
		{
			// Process timed out waiting.
			cout << "process " << pid << " timed out" << endl;
			return -1; // TODO: stałe na errory
		}

		// If we are here - process was awaken, so it got its lock.
		cout << "awaken process " << pid << " locks with type " << request.locktype << " on RID ";
		cout << request.rid << " with timeout " << request.timeout << endl;
		return 0;
	}

	// If we are here - there are no conflicts with active locks, so we can set requested lock.
	Lock lock(request, pid);
	active_locks.push_back(lock);

	cout << "process " << pid << " locks with type " << request.locktype << " on RID ";
	cout << request.rid << " with timeout " << request.timeout << endl;

	return 0;
}

void LockManager::awakeWaiting(rid_t rid)
{
	for (;;)
	{
		if (waiting_locks.empty())
		{
			return;
		}

		WaitingLock waiting_lock = waiting_locks.front();
		TryLockRequest tryRequest =
		{ waiting_lock.request.rid, waiting_lock.request.locktype };
		if (tryLock(tryRequest, waiting_lock.pid))
		{
			// We can't awake him.
			return;
		}

		// Copy lock to active list.
		Lock lock(waiting_lock.request, waiting_lock.pid);
		active_locks.push_back(lock);

		// Remove waiting lock.
		waiting_locks.pop_front();

		cout << "budzimy " << waiting_lock.pid << endl;

		// Wake up.
		pthread_cond_signal(&waiting_lock.cond);

		cout << "po sygnale" << endl;
	}
}

int LockManager::unlock(UnlockRequest request, pid_t pid)
{
	cout << "process " << pid << " unlocks RID " << request.rid << endl;
	for (list<Lock>::iterator it = active_locks.begin(); it != active_locks.end(); ++it)
	{
		if (request.rid == it->request.rid)
		{
			// Remove this active lock.
			active_locks.remove(*it);

			// Awake processes waiting for this RID.
			awakeWaiting(request.rid);

			return 0;
		}
	}
	return -1; // TODO: stałe na errory
}

int LockManager::tryLock(TryLockRequest request, pid_t pid)
{
	cout << "process " << pid << " tries lock " << request.locktype << " on RID " << request.rid << endl;

	// Iterate through all active locks.
	for (list<Lock>::iterator it = active_locks.begin(); it != active_locks.end(); ++it)
	{
		if (request.rid == it->request.rid && !permission[request.locktype][it->request.locktype])
		{
			// There is a conflict with some active lock.
			cout << "you can't have lock" << endl;
			return -1;
		}
	}
	cout << "you can have lock" << endl;
	// There are no conflicts with active locks, so it is possible to set given lock.
	return 0;
}

void LockManager::cleanup(pid_t pid)
{
	cout << "cleanup after process " << pid << endl;
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

LockManager::~LockManager()
{
}
