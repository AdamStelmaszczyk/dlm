/**
 * WaitingLock.h
 *
 * @class WaitingLock
 * @brief The same as normal lock, but with its conditional variable.
 * @date 30-05-2012
 */

#ifndef WAITINGLOCK_H_
#define WAITINGLOCK_H_

#include "Lock.h"
#include "LockRequest.h"

namespace dlm
{

class WaitingLock : public Lock
{
public:
	pthread_cond_t *cond;
	pthread_mutex_t *mutex;

	WaitingLock(LockRequest request, pid_t pid, pthread_cond_t *cond, pthread_mutex_t *mutex);
};

}

#endif /* WAITINGLOCK_H_ */
