#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <unistd.h>

#include "../lock_manager/UnlockRequest.h"
#include "../lock_manager/TryLockRequest.h"

int p_request = 0;	// pipe descriptor number where client puts requests
int p_response = 0; // pipe descriptor number where client puts responses

using namespace dlm;

int DLM_init(int argc, char **argv)
{
	if (argc < 3)
	{
		return -1; // TODO errno
	}
	p_response = atoi(argv[argc - 2]);
	p_request = atoi(argv[argc - 1]);
	if (!p_request || !p_response)
	{
		return -1; // TODO errno
	}
	return 0;
}

/**
 * template to send data to dlm
 *
 * @param data - data to send
 * @return value of dlm procedure call
 */
template<typename T>
int sendMesage(char instr_type, T data)
{
	LockRequest lock_request;
	// write request type to pipe
	if (!p_request || !p_response)
	{
		return -1; // TODO errno - DLM niezainicjowany
	}
	if (write(p_request, &instr_type, sizeof(instr_type)) == -1) // TODO ciagnac w petli while az zapisze wszystko
	{
		return -1; // TODO errno
	}
	if (write(p_request, &lock_request, sizeof(lock_request)) == -1) // TODO ciagnac w petli while az zapisze wszystko
	{
		return -1; // TODO errno
	}
	// read response from pipe
	int result;
	if (read(p_response, &result, sizeof(result)) == -1)
	{
		return -1; // TODO errno
	}
	return result;
}

int DLM_init_file_resource(char** dest)
{
	// TODO implement
	return 0;
}

int DLM_lock(rid_t resource_id, LockType lock_type, time_t timeout)
{
	LockRequest lock_request;
	lock_request.resource_ = resource_id;
	lock_request.locktype_ = lock_type;
	lock_request.timeout_ = timeout;
	return sendMesage('l', lock_request);
}

int DLM_unlock(rid_t resource_id)
{
	UnlockRequest unlock_request;
	unlock_request.resource_ = resource_id;
	return sendMesage('u', unlock_request);
}

int DLM_trylock(rid_t resource_id, LockType lock_type)
{
	TryLockRequest try_request;
	try_request.resource_ = resource_id;
	try_request.lockType_ = lock_type;
	return sendMesage('t', try_request);
}
