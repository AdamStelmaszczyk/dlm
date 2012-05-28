/**
 * Config.h
 * @class Console
 * @brief Provides configuration file handling
 * @date 27-05-2012
 */

#ifndef CONFIG_H_
#define CONFIG_H_
#include <string>
#include <map>

namespace dlm
{

class Config
{
public:
	Config(const std::string& filename);
	virtual ~Config();
	/**
	 * Returns property value
	 *
	 * @param property_name
	 */
	std::string getValue(const std::string& property_name) const;
	void readInput();
private:
	/** Map of property-value pairs*/
	std::map<std::string,std::string> config_map;
	std::string filename_;
	std::string line;
};
}

#endif /* CONFIG_H_ */
