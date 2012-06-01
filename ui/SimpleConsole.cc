#include <cstdlib>
#include <vector>
#include <sstream>
#include <fstream>
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
		in_(in), out_(out), config_(config), lockManager_(lm)
{
	Logger::getInstance().setOutputStream(out);
	pthread_t t;
	cleaner_ = new Cleaner(lm);
	pthread_create(&t, NULL, &start_cleaner, (void*) cleaner_);
}

SimpleConsole::~SimpleConsole()
{
	delete cleaner_;
}

void SimpleConsole::start()
{
	string instr;
	Logger::getInstance().log("dlm console started");
	while (in_)
	{
		try
		{
			getline(in_, instr);
			callProc(instr);
		}
		catch (const Warning &e)
		{
			Logger::getInstance().log("[%s]", e.what());
			continue; // Warnings shows only message, but shouldn't kill dlm
		}
		catch (const std::exception &e)
		{ // other exceptions - here is also Error
			Logger::getInstance().log("[%s]", e.what());
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
		char buff1[256], buff2[256]; // converting descriptor number to cstring
		sprintf(buff1, "%d", p_request[WRITE_DESC]);
		sprintf(buff2, "%d", p_response[READ_DESC]);

		//wydziel nazwę programu i przekazane argumenty
		string param;
		stringstream sstream(dst);

		vector<string> params;

		while (sstream >> param)
			params.push_back(param);

		//2 na nazwę terminala i parametr -e,
		//2 dodatkowe na deskryptory i jeden na NULL
		unsigned param_num = params.size() + 5;

		char * arg[param_num];
		arg[0] = (char*)string(config_.getValue("terminal")).c_str();
		arg[1] = (char*)string("-e").c_str();
		for(unsigned i = 2; i < param_num - 3; ++i)
		{
			arg[i] = (char*)string(params[i-2]).c_str();
		}
		arg[param_num-3] = (char*)buff2;
		arg[param_num-2] = (char*)buff1;
		arg[param_num-1] = NULL;

		execv(string(config_.getValue("terminal")).c_str(),arg);

		throw WARNING2("couldn't open exec file: " + dst, errno);
	}
	else if (child_pid == -1)
	{
		throw WARNING("couldn't create child process");
	}

	close(p_response[READ_DESC]);
	close(p_request[WRITE_DESC]);

	// start listener

	Listener* listener = new Listener(p_response[WRITE_DESC], p_request[READ_DESC], child_pid, lockManager_); // FIXME memory leak
	pthread_t thread; // FIXME zapamietac moze gdzies strukture watku ?
	pthread_create(&thread, NULL, &start_listener, (void*) listener);
	cleaner_->addClient(child_pid, thread);
}

}
