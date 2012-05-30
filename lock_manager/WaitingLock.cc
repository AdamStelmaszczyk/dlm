/**
 * WaitingLock.cc
 *
 * @class WaitingLock
 * @date 30-05-2012
 */

#include "WaitingLock.h"

using namespace dlm;

WaitingLock::WaitingLock(LockRequest request, pid_t pid, pthread_cond_t cond, pthread_mutex_t mutex)
{
	this->request = request;
	this->pid = pid;
	this->cond = cond;
	this->mutex = mutex;
}
