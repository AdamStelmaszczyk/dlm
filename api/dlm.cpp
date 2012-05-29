#include "dlm.h"
#include "../lock_manager/LockRequest.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
// p_request - here client puts requests
// p_response - here client puts responses
int p_request = 0, p_response = 0;

using namespace dlm;

int DLM_init(int argc, char **argv)
{
	if(argc < 3)
		return -1; // TODO errno
	p_response = atoi(argv[argc-2]);
	p_request = atoi(argv[argc-1]);
	if(!p_request || !p_response)
		return -1; // TODO errno
	return 0;
}

int DLM_init_file_resource(char** dest)
{
	// TODO implement
	return 0;
}

int DLM_lock(rid_t resource_id, LockType lock_type, time_t timeout)
{
	LockRequest r;
	char instr_type = 'l';
	r.resource_ = resource_id;
	r.locktype_ = lock_type;
	r.timeout_ = timeout;
	// write lock request to pipe
	if(!p_request || !p_response)
		return -1; // TODO errno - DLM niezainicjowany
	if(write(p_request, &instr_type, sizeof(instr_type)) == -1) // TODO ciagnac w petli while az zapisze wszystko
		return -1; // TODO errno
	if(write(p_request, &r, sizeof(r)) == -1) // TODO ciagnac w petli while az zapisze wszystko
		return -1; // TODO errno
	int result;
	if(read(p_response, &result, sizeof(result)) == -1)
		return -1; // TODO errno
	return result;
}

int DLM_unlock(rid_t resource_id)
{
	// TODO implement
	return 0;
}

int DLM_trylock(rid_t resource_id, LockType lock_type)
{
	// TODO implement
	return 0;
}



