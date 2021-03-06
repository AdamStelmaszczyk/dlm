/**
 * WaitingLock.cc
 *
 * @class WaitingLock
 * @date 30-05-2012
 */

#include "WaitingLock.h"

using namespace dlm;

WaitingLock::WaitingLock(LockRequest request, pid_t pid, pthread_cond_t *cond) :
		Lock(request, pid), cond(cond)
{
}
