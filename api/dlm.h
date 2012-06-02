/**
 * dlm.h
 *
 * @class dlm
 * @brief Distributed Lock Manager declarations.
 * @date 20-05-2012
 */

#ifndef DLM_H_
#define DLM_H_

#define OK					0
#define ALREADY_LOCKED 		1
#define TIMEOUT 			2
#define NO_SUCH_LOCK 		3
#define ALREADY_HAVE_LOCK 	4
#define NOT_DLM_CHILD		5 // there's no pipes in argv
#define DLM_NOT_INITIALIZED 6
#define NO_CONNECTION 		7 // when we can't read/write to pipe


/** Resource id type. */
typedef unsigned int rid_t;

/** Time in ms. */
typedef long time_t;

/** Types of locks. */
enum LockType
{
	CR, // Concurrent Read
	CW, // Concurrent Write
	PR, // Protected Read
	PW, // Protected Write
	EX  // Exclusive
};

/**
 * Initialize dlm client structures.
 * It should be called before use any of dlm functions.
 *
 * @param argc - argc from main (args count)
 * @param argv - argv from main (args values)
 * @return
 */
int DLM_init(int argc, char **argv);

/**
 *	Sends lock resource request to dlm parent.
 *
 * @param resource_id
 * @param lock_type
 * @param timeout 0 == inifinity, -1 == non_block
 * @return // TODO kody powrotu itd.
 */
int DLM_lock(rid_t resource_id, LockType lock_type, time_t timeout);

/**
 *	Sends unlock resource request to dlm parent.
 *
 * @param resource_id
 * @return // TODO kody powrotu itd.
 */
int DLM_unlock(rid_t resource_id);

/**
 * Checks if lock can be provided on resource.
 *
 * @param resource_id
 * @param lock_type
 * @return // TODO kody powrotu itd.
 */
int DLM_trylock(rid_t resource_id, LockType lock_type);

#endif /* DLM_H_ */
