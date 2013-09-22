#ifndef __PROPERTYPAIR_H__
#define __PROPERTYPAIR_H__
#include <string>

class PropertyCollection;
#include "propertycollection.h"


namespace PropertyTypes
{
	enum PropertyTypes
	{
		Bool, Int, Double, String, Table
	};
}

class PropertyPair
{
	public:
		double* getAsDouble();
		int* getAsInt();
		std::string* getAsString();
		bool* getAsBool();
		PropertyCollection* getAsTable();
		
		PropertyPair();
		~PropertyPair();
		
		unsigned char propertyType;
		void* internalData;
		
};

#endif // __PROPERTYPAIR_H__
