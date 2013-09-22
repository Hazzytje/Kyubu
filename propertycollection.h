#ifndef __PROPERTYCOLLECTION_H__
#define __PROPERTYCOLLECTION_H__
#include <string>
#include <map>

class PropertyPair;
#include "propertypair.h"

class PropertyCollection
{
	public:
		void AddInt(std::string key, int value);
		void AddBool(std::string key, bool value);
		void AddDouble(std::string key, double value);
		void AddTable(std::string key, PropertyCollection* value);
		void AddString(std::string key, std::string value);
		
		int GetInt(std::string key);
		bool GetBool(std::string key);
		double GetDouble(std::string key);
		PropertyCollection* GetTable(std::string key);
		std::string GetString(std::string key);
		
		PropertyCollection();
		~PropertyCollection();
	private:
		std::map<std::string, PropertyPair*> properties;
};

#endif // __PROPERTYCOLLECTION_H__
