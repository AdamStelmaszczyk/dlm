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
			vector<string> args = parse_arguments(instr);
			if (args.size() > 0)
			{
				fstream program_file;
				program_file.open(args[0].c_str(), ios::in);
				if (program_file.is_open())
				{
					call_proc(args);
				}
				else
				{
					throw WARNING2("couldn't open exec file: " + args[0], errno);
				}

			}
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

void SimpleConsole::call_proc(vector<string> &args)
{
	int p_response[2]; // output (for dlm) pipe descriptor
	int p_request[2]; // input (for dlm) pipe descriptor
	int child_pid; // pid of called program

	if (pipe(p_response) < 0 || pipe(p_request) < 0)
	{
		throw WARNING2("couldn't create pipe", errno);
	}

	if ((child_pid = fork()) == 0)
	{
		close(p_response[WRITE_DESC]);
		close(p_request[READ_DESC]);

		// Converting descriptor number to c-string.
		char desc_write[32], desc_read[32];
		sprintf(desc_write, "%d", p_request[WRITE_DESC]);
		sprintf(desc_read, "%d", p_response[READ_DESC]);

		// + 5 because 1 for terminal name, 1 for "-e", 2 arguments for descriptors and 1 for NULL.
		int arg_num = args.size() + 5;

		char *terminal_arg[arg_num];
		char *terminal = (char*) config_.getValue("terminal").c_str();

		terminal_arg[0] = terminal;
		terminal_arg[1] = (char*) "-e";

		for (int i = 2; i < arg_num - 3; ++i)
		{
			terminal_arg[i] = (char*) string(args[i - 2]).c_str();
		}

		terminal_arg[arg_num - 3] = (char*) desc_read;
		terminal_arg[arg_num - 2] = (char*) desc_write;
		terminal_arg[arg_num - 1] = NULL;

		cout << terminal << " " << endl;
		for (int i = 0; i < arg_num; i++)
		{
			cout << terminal_arg[i] << endl;
		}

		execv(terminal, terminal_arg);
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

vector<string> SimpleConsole::parse_arguments(string &args)
{
	vector<string> program_args;
	string param;
	stringstream sstream(args);

	while (sstream >> param)
	{
		program_args.push_back(param);
	}

	return program_args;
}

}
