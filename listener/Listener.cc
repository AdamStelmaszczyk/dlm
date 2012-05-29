// TODO obsluga sygnalow SIG_CHLD i podobnych - chyba juz zrobione mozna skasowac?

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

using namespace std;

namespace dlm
{

Listener::Listener(int p_response, int p_request, pid_t client, LockManager& lm) :
		p_response_(p_response), p_request_(p_request), client_(client), lockManager_(lm)
{
}

void Listener::start()
{
	char request_type = 0;
	unsigned size = 0;
	Logger::getInstance().log("[%s]", "new instance of process");
	while (1)
	{
		try
		{
			// first of all, read request message header
			while ((size = read(p_request_, &request_type, sizeof(request_type))) == 0)
				;
			if (size == 0)
			{
				throw ERROR2("Couldn't read request message header", errno);
			}
			Logger::getInstance().log("[%s]", "got new message from process");

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
		catch (const Warning &w)
		{
			Logger::getInstance().log(w.what());
			continue;
		}
		catch (const Error &e)
		{
			Logger::getInstance().log(e.what());
			break;
		}
	}
	Logger::getInstance().log("[%s]", "listener stopped");
}

Listener::~Listener()
{
}

void Listener::handleLockRequest()
{
	LockRequest request;
	// now we read args for LockManager
	if (read(p_request_, &request, sizeof(LockRequest)) == -1)
	{
		throw ERROR2("Couldn't read lock message from pipe", errno);
	}
	int result = lockManager_.lock(request, client_);
	sendResponse(result);
}

void Listener::handleTryLockRequest()
{
	TryLockRequest request;
	// now we read args for LockManager
	if (read(p_request_, &request, sizeof(TryLockRequest)) == -1)
	{
		throw ERROR2("Couldn't read trylock message from pipe", errno);
	}
	int result = lockManager_.tryLock(request, client_);
	sendResponse(result);
}

void Listener::handleUnlockRequest()
{
	UnlockRequest request;
	// now we read args for LockManager
	if (read(p_request_, &request, sizeof(UnlockRequest)) == -1)
	{
		throw ERROR2("Couldn't read unlock message from pipe", errno);
	}
	int result = lockManager_.unlock(request, client_);
	sendResponse(result);
}

void Listener::sendResponse(int result)
{
	if (write(p_response_, &result, sizeof(int)) == -1)
	{
		throw ERROR("Couldn't respond to client");
	}
}

void* start_listener(void *ptr)
{
	// here starts new thread
	pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
	Listener *listener = (Listener*) ((((ptr))));
	listener->start();
	return NULL;
}

}
