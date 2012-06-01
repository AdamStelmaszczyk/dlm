/**
 * Config.h
 * @class Config
 * @brief Provides configuration file handling
 * @date 27-05-2012
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#include <string>
#include <map>

#define SEPARATOR '='

namespace dlm
{

class Config
{
public:
	Config(const std::string filename);
	virtual ~Config();
	/**
	 * Returns given property value.
	 *
	 * @param property_name Name of the property.
	 */
	std::string get_value(const std::string& property_name) const;
private:
	/** Map of property-value pairs*/
	std::map<std::string, std::string> config_map;
	const std::string FILENAME;

	void readInput();
};

}

#endif /* CONFIG_H_ */
