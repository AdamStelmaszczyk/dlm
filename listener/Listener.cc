/**
 * Listener.cc
 *
 * @class Listener
 * @brief Brief description.
 * @author nieznany
 * @date 20-05-2012
 */

#include <iostream>

#include "Listener.h"
#include "../exceptions/Error.h"

using namespace std;

namespace dlm
{

Listener::Listener(int p_response, int p_request) :
		p_response_(p_response), p_request_(p_request)
{
}

void Listener::start()
{
	char buff[256];
	while(read(p_request_, (void*) buff, 5))
	{
		std::cout << buff << std::endl;
	}
	std::cout << "koniec procesu klienckiego" << std::endl;
}

Listener::~Listener()
{
}

void* start_listener(void *ptr)
{
	// here starts new thread
	Listener *listener = (Listener*)(ptr);
	listener->start();

	return NULL;
}

}

/* namespace dlm */
