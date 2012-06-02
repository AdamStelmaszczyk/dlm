#include <iostream>
#include <fstream>
#include <sstream>

#include "Config.h"

using namespace std;
using namespace dlm;

Config::Config(const std::string filename) :
		FILENAME(filename)
{
	readInput();
}

Config::~Config()
{
}

std::string Config::get_value(const std::string& property_name) const
{
	map<string, string>::const_iterator iterator = config_map.find(property_name);
	if (iterator == config_map.end())
	{
		return "NO SUCH PROPERTY KEY";
	}
	return iterator->second;
}

void Config::readInput()
{
	std::string line;
	std::ifstream config_file(FILENAME.c_str());

	if (!config_file.is_open())
	{
		cout << "CAN'T OPEN CONFIG FILE NAMED " << FILENAME << endl;
		return;
	}

	while (getline(config_file, line))
	{
		size_t pos = line.find(SEPARATOR);

		std::string property = line.substr(0, pos);
		std::string value = line.substr(pos + 1, line.length());

		// trim
		property = property.substr(property.find_first_not_of(' '), property.find_last_not_of(' ') + 1);
		value = value.substr(value.find_first_not_of(' '), value.find_last_not_of(' ') + 1);

		config_map[property] = value;
	}

	config_file.close();
}
