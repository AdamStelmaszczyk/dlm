#include "Config.h"
#include <iostream>
#include <fstream>

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

}

}
