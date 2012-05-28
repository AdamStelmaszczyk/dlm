#include <cstdlib>
#include <pthread.h>

#include "SimpleConsole.h"
#include "../exceptions/Warning.h"
#include "../exceptions/Error.h"
#include "../exceptions/Warning.h"
#include "../listener/Listener.h"
#include "../logger/Logger.h"

using namespace std;

namespace dlm
{

SimpleConsole::SimpleConsole(istream &in, ostream &out, Config &config, LockManager &lm) :
		in_(in), out_(out), config_(config),lockManager_(lm)
{
	Logger::getInstance().setOutputStream(out);
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
			Logger::getInstance().log(e.what());
			continue; // Warnings shows only message, but shouldn't kill dlm
		}
		catch (const std::exception &e)
		{ // other exceptions - here is also Error
			Logger::getInstance().log(e.what());
			exit(-1); // we don't know what to do, let's kill him (dlm)
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
		throw WARNING("couldn't create pipe"); // TODO uzupelnic o nr bledu z errno
	}
	if ((child_pid = fork()) == 0)
	{
		close(p_response[WRITE_DESC]);
		close(p_request[READ_DESC]);
		char buff[256]; // converting descriptor number to cstring
		sprintf(buff, "%d", p_request[WRITE_DESC]);
		// lepiej uzyj execv
		// deskryptory na koncu argumentow powinny byc przekazywane
		// w takiej kolejnosci jak jest teraz
		execl(dst.c_str(), dst.c_str(), buff, p_response[READ_DESC], p_request[WRITE_DESC], (void*)0); // last two argvs should be pipe descriptors
		throw WARNING("couldn't open exec file: " + dst);
	}
	else if (child_pid == -1)
		throw WARNING("couldn't create child process");
	close(p_response[READ_DESC]);
	close(p_request[WRITE_DESC]);
	// start listener
	Listener* listener = new Listener(p_response[WRITE_DESC], p_request[READ_DESC], child_pid, lockManager_);
	pthread_t thread; // FIXME zapamietac moze gdzies strukture watku ?
	pthread_create(&thread, NULL, &start_listener, (void*) listener);
}

} /* namespace dlm */
