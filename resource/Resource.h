/**
 * Resource.h
 *
 * @class Resource
 * @brief Abstract class of resource.
 * @date 20-05-2012
 */

#ifndef RESOURCE_H_
#define RESOURCE_H_

namespace dlm
{

class Resource
{
public:
	/// TYPEDEFS
	/** resource id type */
	typedef unsigned int rid_t;

	Resource();
	virtual ~Resource();
};

} /* namespace dlm */
#endif /* RESOURCE_H_ */
