/**
 * Lock.cc
 *
 * @class Lock
 * @brief Class storing info about lock.
 * @date 30-05-2012
 */

#include "Lock.h"

using namespace dlm;

Lock::Lock(LockRequest request, pid_t pid)
{
	this->request = request;
	this->pid = pid;
}

bool Lock::operator==(const Lock& other) const
{
	return (pid == other.pid) && (request.rid == other.request.rid);
}
