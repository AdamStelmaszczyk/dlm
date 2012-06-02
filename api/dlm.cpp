#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <errno.h>

#include "dlm.h"
#include "../lock_manager/LockRequest.h"
#include "../lock_manager/UnlockRequest.h"
#include "../lock_manager/TryLockRequest.h"

int p_request = 0; // Pipe descriptor number where client puts requests.
int p_response = 0; // Pipe descriptor number where client puts responses.

using namespace dlm;

int DLM_init(int argc, char *argv[])
{
	if (argc < 3)
	{
		return NOT_DLM_CHILD;
	}
	p_response = atoi(argv[argc - 2]);
	p_request = atoi(argv[argc - 1]);
	if (!p_request || !p_response)
	{
		return NOT_DLM_CHILD;
	}
	return 0;
}

/**
 * Template to send data to DLM.
 *
 * @param data Data to send.
 * @return Value of dlm procedure call.
 */
template <typename T>
int send_message(char instr_type, const T data)
{
	// Write request type to pipe.
	if (!p_request || !p_response)
	{
		return DLM_NOT_INITIALIZED;
	}
	if (write(p_request, &instr_type, sizeof(instr_type)) < (int) sizeof(instr_type))
	{
		return NO_CONNECTION;
	}
	if (write(p_request, &data, sizeof(T)) < (int) sizeof(T))
	{
		return NO_CONNECTION;
	}
	// Read response from pipe.
	int result;
	if (read(p_response, &result, sizeof(result)) < (int) sizeof(result))
	{
		return NO_CONNECTION;
	}
	return result;
}

int DLM_lock(rid_t resource_id, LockType lock_type, time_t timeout)
{
	LockRequest lock_request;
	lock_request.rid = resource_id;
	lock_request.locktype = lock_type;
	lock_request.timeout = timeout;
	return send_message('l', lock_request);
}

int DLM_unlock(rid_t resource_id)
{
	UnlockRequest unlock_request;
	unlock_request.rid = resource_id;
	return send_message('u', unlock_request);
}

int DLM_trylock(rid_t resource_id, LockType lock_type)
{
	TryLockRequest try_request;
	try_request.rid = resource_id;
	try_request.locktype = lock_type;
	return send_message('t', try_request);
}
