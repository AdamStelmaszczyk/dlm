/**
 * Listener.h
 *
 * @class Listener
 * @brief Communicates with clients.
 * @date 20-05-2012
 */

#ifndef LISTENER_H_
#define LISTENER_H_

#include "../lock_manager/LockManager.h"

namespace dlm
{

class Listener
{
public:
	Listener(int p_response, int p_request, pid_t client, LockManager &lockManager);
	void start();
	virtual ~Listener();
private:
	/**
	 * Request handlers.
	 */
	void handleLockRequest();
	void handleTryLockRequest();
	void handleUnlockRequest();

	/**
	 * Puts response to output pipe.
	 *
	 * @param result Response.
	 */
	void sendResponse(int result);

	/** Pipe's for request (from client) and response (to client). */
	int p_response, p_request;
	/** PID of child process (client) communicating with this listener. */
	pid_t client;
	/** Reference to lock manager providing DLM functions. */
	LockManager &lock_manager;
};

}

void* start_listener(void *ptr);

#endif /* LISTENER_H_ */
