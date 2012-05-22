#include <cstdlib>
#include <pthread.h>

#include "SimpleConsole.h"
#include "../exceptions/Warning.h"
#include "../exceptions/Error.h"
#include "../listener/Listener.h"

using namespace std;

namespace dlm
{

SimpleConsole::SimpleConsole(istream &in, ostream &out) :
		in_(in), out_(out)
{
}

SimpleConsole::~SimpleConsole()
{
}

void SimpleConsole::start()
{
	string instr;
	while (in_)
	{
		try
		{
			in_ >> instr;
			callProc(instr);
		}
		catch (const Warning &e)
		{
			out_ << e.what() << endl; // TODO uzyc loggera
			continue;
		}
		catch (const std::exception &e)
		{ // other exceptions
			out_ << e.what() << endl;
			exit(-1);
		}
	}
}

void SimpleConsole::callProc(const std::string &dst)
{
	int p_response[2]; // output (for dlm) pipe descriptor
	int p_request[2]; // input (for dlm) pipe descriptor
	int child_pid; // pid of called program

	if (pipe(p_response) < 0 || pipe(p_request) < 0)
	{
		throw WARNING("couldn't create pipe"); // TODO uzupelnic o nr bledu
	}
	if ((child_pid = fork()) == 0)
	{
		close(p_response[WRITE_DESC]);
		close(p_request[READ_DESC]);
		char buff[256];
		sprintf(buff, "%d", p_request[WRITE_DESC]);
		execl(dst.c_str(), dst.c_str(), buff, (char*) 0); // TODO poprawic pierwszy argument (sama nazwa programu), dodac dodatkowe argumenty
		throw WARNING("couldn't open exec file: " + dst);
	}
	else if (child_pid == -1)
		throw WARNING("couldn't create child process");
	close(p_response[READ_DESC]);
	close(p_request[WRITE_DESC]);
	// start listener
	Listener* listener = new Listener(p_response[WRITE_DESC], p_request[READ_DESC]);
	pthread_t thread; // FIXME zapamietac moze gdzies strukture watku
	pthread_create(&thread, NULL, &start_listener, (void*) listener);
}

} /* namespace dlm */
