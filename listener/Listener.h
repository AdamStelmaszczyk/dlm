/**
 * Listener.h
 *
 * @class Listener
 * @brief Brief description.
 * @date 20-05-2012
 */

#ifndef LISTENER_H_
#define LISTENER_H_

#include "../lock_manager/LockManager.h"
namespace dlm
{
void* start_listener(void *ptr);

class Listener
{
public:
	Listener(int p_response, int p_request, pid_t client, LockManager &lockManager);
	void start();
	virtual ~Listener();
private:
	/**
	 * request handlers
	 */
	void handleLockRequest();
	void handleTryLockRequest();
	void handleUnlockRequest();

	/**
	 * Puts response to output pipe.
	 *
	 * @param result
	 */
	void sendResponse(int result);

	/** pipe's for request (from client) and response (to client) */
	int p_response_, p_request_;
	/** reference to lock manager providing dlm functions */
	LockManager &lockManager_;
	/** pid of child process (client) communicatig with this listner */
	pid_t client_;

	friend void* start_listener(void *ptr);
};

} /* namespace dlm */
#endif /* LISTENER_H_ */
