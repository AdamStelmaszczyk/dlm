/**
 * Listener.cc
 *
 * @class Listener
 * @brief Brief description.
 * @date 20-05-2012
 */

#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#include "Listener.h"
#include "../exceptions/Error.h"
#include "../exceptions/Warning.h"
#include "../logger/Logger.h"

using namespace std;

namespace dlm
{

Listener::Listener(int p_response, int p_request) :
		p_response_(p_response), p_request_(p_request)
{
}

void Listener::start()
{
	char request_type = 0;
	while (1)
	{
		try
		{
			// first of all, read request message header
			if (read(p_request_, &request_type, 1) == -1)
				throw ERROR("Couldn't read request message header", errno);
			// if-else-if, grrr ...
			if (request_type == 'l')
				handleLockRequest();
			else if (request_type == 't')
				handleTryLockRequest();
			else if (request_type == 'u')
				handleUnlockRequest();

		}
		catch (const Warning &e)
		{
			Logger::getInstance().log(e.what());
		}
		catch (const Error &e)
		{
			Logger::getInstance().log(e.what());
			pthread_exit(NULL); // stop that thread
		}
	}
}

Listener::~Listener()
{
}

void Listener::handleLockRequest()
{
	// first should be r_id
if()
}

void Listener::handleTryLockRequest()
{
}

void Listener::handleUnlockRequest()
{
}

void *start_listener(void *ptr)
{
	// here starts new thread
	Listener *listener = (Listener*) ((ptr));
	listener->start();
	return NULL;
}

/* namespace dlm */
}

