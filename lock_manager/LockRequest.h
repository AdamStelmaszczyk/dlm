/**
 * LockRequest.h
 *
 * @class LockRequest
 * @brief structure storing info about lock request
 * @date 28-05-2012
 */

#ifndef LOCKREQUEST_H_
#define LOCKREQUEST_H_
#include "../api/dlm.h"

namespace dlm
{

struct LockRequest
{
	time_t timeout_;
	rid_t resource_;
	LockType locktype_;
};

} /* namespace dlm */
#endif /* LOCKREQUEST_H_ */
