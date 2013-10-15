#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <map>

class Config
{
public:
	Config(std::string fileName);
	~Config();
	
	void Reload();
	
	std::string GetString (std::string keyName);
	bool        GetBool   (std::string keyName);
	int         GetInt    (std::string keyName);
	double      GetDouble (std::string keyName);
	
	void SetString (std::string keyName, std::string value);
	void SetBool   (std::string keyName, bool value);
	void SetInt    (std::string keyName, int value);
	void SetDouble (std::string keyName, double value);
private:
	std::string fileName;
	std::map<std::string, std::string> stringContents;
	std::map<std::string, int> intBoolContents;
	std::map<std::string, double> doubleContents;
};

#endif