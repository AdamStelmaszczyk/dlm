// TODO obsluga sygnalow SIG_CHLD i podobnych

#include <pthread.h>
#include <iostream>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>

#include "../lock_manager/LockRequest.h"
#include "../lock_manager/UnlockRequest.h"
#include "../lock_manager/TryLockRequest.h"
#include "../api/dlm.h"
#include "Listener.h"
#include "../exceptions/Error.h"
#include "../exceptions/Warning.h"
#include "../logger/Logger.h"

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
	Logger::getInstance().log("[%s]","new instance of process");
	while (1)
	{
		try
		{
			// first of all, read request message header
			while ((size = read(p_request_, &request_type, sizeof(request_type))) == 0)
				;
			if (size == 0)
				throw ERROR2("Couldn't read request message header", errno);
			Logger::getInstance().log("[%s]","got new message from process");
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
			continue;
		}
		catch (const Error &e)
		{
			Logger::getInstance().log(e.what());
			break;
		}
	}
	Logger::getInstance().log("[%s]","listener stoped");
}

Listener::~Listener()
{
}

void Listener::handleLockRequest()
{
	LockRequest r;
	// now we read args for LockManager
	if (read(p_request_, &r, sizeof(LockRequest)) == -1)
		throw ERROR2("Couldn't read lock message from pipe", errno);
	int result = lockManager_.lock(r, client_);
	sendResponse(result);
}

void Listener::handleTryLockRequest()
{
	TryLockRequest r;
	// now we read args for LockManager
	if (read(p_request_, &r, sizeof(TryLockRequest)) == -1)
		throw ERROR2("Couldn't read trylock message from pipe", errno);
	int result = lockManager_.tryLock(r, client_);
	sendResponse(result);
}

void Listener::handleUnlockRequest()
{
	UnlockRequest r;
	// now we read args for LockManager
	if (read(p_request_, &r, sizeof(UnlockRequest)) == -1)
		throw ERROR2("Couldn't read unlock message from pipe", errno);
	int result = lockManager_.unlock(r, client_);
	sendResponse(result);
}

void Listener::sendResponse(int result)
{
	if (write(p_response_, &result, sizeof(int)) == -1)
		throw ERROR("Couldn't send resposne to client");
}

void *start_listener(void *ptr)
{
	// here starts new thread
	Listener *listener = (Listener*) ((((ptr))));
	listener->start();
	return NULL;
}

/* namespace dlm */
}

