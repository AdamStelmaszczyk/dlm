/**
 * LockOwner.h
 *
 * @class LockOwner
 * @brief Predicate for remove_if.
 * @date 30-05-2012
 */

#ifndef LOCKOWNER_H_
#define LOCKOWNER_H_

#include <sys/types.h>

#include "Lock.h"

namespace dlm
{

class LockOwner
{
public:
	LockOwner(pid_t pid);
	bool operator()(const Lock& lock);
private:
	pid_t pid;
};

}

#endif /* LOCKOWNER_H_ */
