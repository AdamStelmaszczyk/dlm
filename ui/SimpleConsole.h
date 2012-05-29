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
	void callProc(const std::string &dst); // TODO uzupelnic o argumenty
	/** input stream */
	std::istream& in_;
	/** output stream */
	std::ostream& out_;
	/** config object */
	Config& config_;
	/** lock manager for child process */
	LockManager &lockManager_;
	/** cleaner to cleanup after dead childs */
	Cleaner* cleaner_;
};

} /* namespace dlm */
#endif /* SIMPLECONSOLE_H_ */
