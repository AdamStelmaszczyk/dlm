/**
 * Lock.h
 *
 * @class Lock
 * @brief Class storing info about lock.
 * @date 30-05-2012
 */

#ifndef LOCK_H_
#define LOCK_H_

#include <sys/types.h>

#include "LockRequest.h"

namespace dlm
{

class Lock
{
public:
	LockRequest request;
	pid_t pid;

	Lock();
	Lock(LockRequest request, pid_t pid);
	bool operator==(const Lock& other) const;
};

}

#endif /* LOCK_H_ */
