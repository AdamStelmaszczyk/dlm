/**
 * LockOwner.cc
 *
 * @class LockOwner
 * @date 30-05-2012
 */

#include "LockOwner.h"

using namespace dlm;

LockOwner::LockOwner(pid_t pid)
{
	this->pid = pid;
}

bool LockOwner::operator()(const Lock& lock)
{
	return lock.pid == pid;
}
