/**
 * dlm.h
 *
 * @class dlm
 * @brief Distributed Lock Manager declarations.
 * @author nieznany
 * @date 20-05-2012
 */

// TODO list:

#ifndef DLM_H_
#define DLM_H_

/// TYPEDEFS
/** resource id type */
typedef unsigned int rid_t;
/** time type */
typedef long time_t;
/** types of locks */
enum LockType
{
	CR, // Concurrent Read
	CW, // Concurrent Write
	PR, // Protected Read
	PW, // Protected Write
	EX  // Exclusive
};

///	FUNCTIONS

/**
 * Initialize dlm client structures.
 * It should be called before use any of dlm functions.
 *
 * @param argc - argc from main (args count)
 * @param argv - argv from main (args values)
 * @return // TODO ustalic: 0 lub jakis kod bledu.
 */
// przede wszystkimi powinno inicjalizowac zmienna globalna / singleton
// danych przechwoujacych deskryptor do pisania do dlm
int DLM_init(int argc, char **argv);

/**
 * Checks wheter file exists and creates new resource.
 *
 * @return id of resource or -1 if
 */
// ew to mozna sobie odpuscic - zasob w dlm o podanym id bedzie
// tworzony w momencie pierwszego wywolania dlm_lock, tylko to wtedy
// jest malo praktyczne (procesy potomne (klienci) musza znac id
// poszczegolnych zasobow)
int DLM_init_file_resource(char** dest);

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
 * @return
 */
int DLM_trylock(rid_t resource_id, LockType lock_type);
#endif /* DLM_H_ */
