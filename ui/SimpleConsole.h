/**
 * SimpleConsole.h
 *
 * @class SimpleConsole
 * @brief Brief description.
 * @date 20-05-2012
 */

#ifndef SIMPLECONSOLE_H_
#define SIMPLECONSOLE_H_
#include <iostream>
#include <string>
#include <vector>
#include "../lock_manager/LockManager.h"
#include "../config/Config.h"
#include "../listener/Cleaner.h"

#define READ_DESC 0
#define WRITE_DESC 1

namespace dlm
{

class SimpleConsole
{
public:
	SimpleConsole(std::istream &in, std::ostream &out, Config &config, LockManager &lm);
	virtual ~SimpleConsole();

	/**
	 * starts console, waits for input
	 * FIXME na razie tylko po prostu wolanie programu bez parametrow
	 */
	void start();
private:
	/**
	 * calls new process and listener for that process
	 *
	 * @param dst - string to call
	 */
	void call_proc(std::vector<std::string> &args);
	/**
	 * parse arguments passed to console
	 *
	 * @param args - string of arguments
	 */
	std::vector<std::string> parse_arguments(std::string &args);
	/** input stream */
	std::istream& in;
	/** output stream */
	std::ostream& out;
	/** config object */
	Config& config;
	/** lock manager for child process */
	LockManager &lock_manager;
	/** cleaner to cleanup after dead childs */
	Cleaner* cleaner_;
};

} /* namespace dlm */
#endif /* SIMPLECONSOLE_H_ */
