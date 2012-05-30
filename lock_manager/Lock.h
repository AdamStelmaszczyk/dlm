/**
 * Lock.h
 *
 * @class Lock
 * @brief Structure storing info about lock.
 * @date 30-05-2012
 */

#ifndef LOCK_H_
#define LOCK_H_

#include "LockRequest.h"

namespace dlm
{

typedef struct
{
	pid_t pid;
	LockRequest request;
} Lock;

}

#endif /* LOCK_H_ */
