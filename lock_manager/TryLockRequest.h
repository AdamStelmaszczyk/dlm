/**
 * TryLockRequest.h
 *
 * @class TryLockRequest
 * @brief trylock info structure
 * @date 30-05-2012
 */

#ifndef TRYLOCKREQUEST_H_
#define TRYLOCKREQUEST_H_

#include "../api/dlm.h"

namespace dlm
{

struct TryLockRequest
{
	rid_t rid;
	LockType locktype;
};

}

#endif /* TRYLOCKREQUEST_H_ */
