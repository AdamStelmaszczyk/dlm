/**
 * TryLockRequest.h
 *
 * @class TryLockRequest
 * @brief trylock info structure
 * @date 28-05-2012
 */

#ifndef TRYLOCKREQUEST_H_
#define TRYLOCKREQUEST_H_

#include "../api/dlm.h"

namespace dlm
{

struct TryLockRequest
{
	rid_t resource_;
	LockType lockType_;
};

} /* namespace dlm */
#endif /* TRYLOCKREQUEST_H_ */
