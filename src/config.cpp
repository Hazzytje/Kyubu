#include "config.h"

#include <fstream>

Config::Config(std::string fileName)
:fileName(fileName)
{
	Reload();
}

Config::~Config()
{
	//TODO: write ALL the things
}

void Config::Reload()
{
	std::ifstream fileStream(fileName);
	
	std::string keyName;
	fileStream >> keyName;
	
	if(fileStream.peek() == ' ') fileStream.ignore();
	if(fileStream.peek() == '=') fileStream.ignore();
	if(fileStream.peek() == ' ') fileStream.ignore();
	
	char peeked = fileStream.peek();
	if(peeked == 't' || peeked == 'f')
	{
		intBoolContents[keyName] = peeked == 't';
		fileStream.ignore(100, '\n');
	}
	else if(peeked == '\"')
	{
		//eat the "
		fileStream.ignore();
		std::string value;
		fileStream >> value;
		//remove the last "
		value.erase(value.end() - 1);
		stringContents[keyName] = value;
	}
	else
	{
		std::string intOrDouble;
		fileStream >> intOrDouble;
		if (intOrDouble.find('.') == std::string::npos)
		{
			//it's an int
			intBoolContents[keyName] = atoi(intOrDouble.c_str());
		}
		else
		{
			//it's a double
			doubleContents[keyName] = atof(intOrDouble.c_str());
		}
	}
}

std::string Config::GetString (std::string keyName)
{
	return stringContents[keyName];
}
bool        Config::GetBool   (std::string keyName)
{
	return intBoolContents[keyName];
}
int         Config::GetInt    (std::string keyName)
{
	return intBoolContents[keyName];
}
double      Config::GetDouble (std::string keyName)
{
	return doubleContents[keyName];
}

void Config::SetString (std::string keyName, std::string value)
{
	stringContents[keyName] = value;
}
void Config::SetBool   (std::string keyName, bool value)
{
	intBoolContents[keyName] = value;
}
void Config::SetInt    (std::string keyName, int value)
{
	intBoolContents[keyName] = value;
}
void Config::SetDouble (std::string keyName, double value)
{
	doubleContents[keyName] = value;
}