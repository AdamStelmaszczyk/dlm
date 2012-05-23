/**
 * LockManager.h
 *
 * @class LockManager
 * @brief Stores info about locks, provides methods
 *  	  to lock and unlock resource.
 * @date 20-05-2012
 */

#ifndef LOCKMANAGER_H_
#define LOCKMANAGER_H_

#include <queue>

#include "../resource/Resource.h"

namespace dlm
{

class LockManager
{
public:
	/// TYPEDEFS
	typedef unsigned int rid_t;
	void lock(int resource_id, int lock_type, long timeout);
	LockManager();
	virtual ~LockManager();
private:

};

} /* namespace dlm */
#endif /* LOCKMANAGER_H_ */
