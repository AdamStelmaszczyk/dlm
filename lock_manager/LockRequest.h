/**
 * LockRequest.h
 *
 * @class LockRequest
 * @brief Structure storing info about lock request.
 * @date 30-05-2012
 */

#ifndef LOCKREQUEST_H_
#define LOCKREQUEST_H_

#include "../api/dlm.h"

namespace dlm
{

typedef struct
{
	time_t timeout;
	rid_t rid;
	LockType locktype;
} LockRequest;

}

#endif /* LOCKREQUEST_H_ */
