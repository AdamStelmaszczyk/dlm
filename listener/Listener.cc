#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#include "Listener.h"
#include "../api/dlm.h"
#include "../logger/Logger.h"
#include "../exceptions/Error.h"
#include "../exceptions/Warning.h"
#include "../lock_manager/LockRequest.h"
#include "../lock_manager/UnlockRequest.h"
#include "../lock_manager/TryLockRequest.h"
#include "Cleaner.h"

using namespace std;
using namespace dlm;

Listener::Listener(int p_response, int p_request, pid_t client, LockManager& lm, Cleaner& cleaner) :
		p_response(p_response), p_request(p_request), client(client), lock_manager(lm), cleaner(cleaner)
{
}

void Listener::start()
{
	char request_type = 0;
	Logger::getInstance().log("[%s: %d]", "new instance of process", client);
	while (1)
	{
		try
		{
			// first of all, read request message header
			if (read(p_request, &request_type, sizeof(request_type)) < (int) sizeof(request_type))
			{
				break; // pipe was closed
			}
			Logger::getInstance().log("[%s: %c]", "got new message from process", request_type);

			switch (request_type)
			{
				case 'l':
					handleLockRequest();
					break;
				case 't':
					handleTryLockRequest();
					break;
				case 'u':
					handleUnlockRequest();
					break;
			}
		}
		catch (const Error &e)
		{
			char message[256];
			Logger::getInstance().log(e.what(message));
			break;
		}
		catch (const Warning &w)
		{
			char message[256];
			Logger::getInstance().log(w.what(message));
			continue;
		}
	}
	close(p_request);
	close(p_response);
	Logger::getInstance().log("[%s]", "listener stopped");
}

Listener::~Listener()
{
}

void Listener::handleLockRequest()
{
	LockRequest request;
	// Read args for LockManager.
	if (read(p_request, &request, sizeof(LockRequest)) < (int) sizeof(LockRequest))
	{
		throw ERROR2("Couldn't read lock message from pipe", errno);
	}
	int result = lock_manager.lock(request, client);
	sendResponse(result);
}

void Listener::handleTryLockRequest()
{
	TryLockRequest request;
	// Read args for LockManager.
	if (read(p_request, &request, sizeof(TryLockRequest)) < (int) sizeof(TryLockRequest))
	{
		throw ERROR2("Couldn't read trylock message from pipe", errno);
	}
	int result = lock_manager.tryLock(request, client);
	sendResponse(result);
}

void Listener::handleUnlockRequest()
{
	UnlockRequest request;
	// Read args for LockManager.
	if (read(p_request, &request, sizeof(UnlockRequest)) < (int) sizeof(UnlockRequest))
	{
		throw ERROR2("Couldn't read unlock message from pipe", errno);
	}
	int result = lock_manager.unlock(request, client);
	sendResponse(result);
}

void Listener::sendResponse(int result)
{
	if (write(p_response, &result, sizeof(int)) < (int) sizeof(int))
	{
		throw ERROR("Couldn't respond to client");
	}
}

void* start_listener(void *ptr)
{
	// Starts new thread.
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	Listener *listener = (Listener*) ptr;
	listener->start();
	return NULL;
}
