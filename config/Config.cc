#include "Config.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
namespace dlm
{
Config::Config(const std::string& filename) : filename_(filename)
{

}

Config::~Config()
{

}

std::string Config::getValue(const std::string& property_name) const
{
	if(config_map.count(property_name) > 0)
	{
		return config_map.find(property_name)->second;
	}
	else
	{
		return "";
	}
}

void Config::readInput()
{
    std::string line;
    std::ifstream config_file(filename_.c_str());

    if(config_file.is_open())
    {
            while(getline(config_file, line))
            {
                    size_t pos = line.find(':');

                    std::string property = line.substr(0, pos);
                    std::string value = line.substr(pos + 1, line.length());

                    //trim
                    property = property.substr(property.find_first_not_of(' '), property.find_last_not_of(' ') + 1);
                    value = value.substr(value.find_first_not_of(' '), value.find_last_not_of(' ') + 1);

                    config_map[property] = value;
            }

    }

    config_file.close();

}

}
