/**
 * Listener.h
 *
 * @class Listener
 * @brief Brief description.
 * @date 20-05-2012
 */

#ifndef LISTENER_H_
#define LISTENER_H_

namespace dlm
{
void* start_listener(void *ptr);

class Listener
{
public:
	Listener(int p_response, int p_request);
	void start();
	virtual ~Listener();
private:
	/** pipe's for request (from client) and response (to client) */
	int p_response_, p_request_;

	friend void* start_listener(void *ptr);
};

} /* namespace dlm */
#endif /* LISTENER_H_ */
