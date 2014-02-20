#include "propertypair.h"

PropertyPair::PropertyPair()
{
	internalData = NULL;
}

PropertyPair::~PropertyPair()
{
	if(internalData != NULL)
	{
		if (propertyType == PropertyTypes::Double)
		{
			delete reinterpret_cast<double*>(internalData);
		}
		if (propertyType == PropertyTypes::Int)
		{
			delete reinterpret_cast<int*>(internalData);
		}
		if (propertyType == PropertyTypes::String)
		{
			delete reinterpret_cast<std::string*>(internalData);
		}
		if (propertyType == PropertyTypes::Bool)
		{
			delete reinterpret_cast<bool*>(internalData);
		}
		if (propertyType == PropertyTypes::Table)
		{
			delete reinterpret_cast<PropertyCollection*>(internalData);
		}
	}
}

using std::string;

double* PropertyPair::getAsDouble()
{
	if(propertyType == PropertyTypes::Double)
	{
		return reinterpret_cast<double*>(internalData);
	}
	else
	{
		propertyType = PropertyTypes::Double;
		return reinterpret_cast<double*>(internalData);
	}
}

int* PropertyPair::getAsInt()
{
	if(propertyType == PropertyTypes::Int )
	{
		return reinterpret_cast<int*>(internalData);
	}
	else
	{
		propertyType = PropertyTypes::Int;
		return reinterpret_cast<int*>(internalData);
	}
}

string* PropertyPair::getAsString()
{
	if(propertyType == PropertyTypes::String )
	{
		return reinterpret_cast<string*>(internalData);
	}
	else
	{
		propertyType = PropertyTypes::String;
		return reinterpret_cast<string*>(internalData);
	}
}

bool* PropertyPair::getAsBool()
{
	if(propertyType == PropertyTypes::Bool )
	{
		return reinterpret_cast<bool*>(internalData);
	}
	else
	{
		propertyType = PropertyTypes::Bool;
		return reinterpret_cast<bool*>(internalData);
	}
}

PropertyCollection* PropertyPair::getAsTable()
{
	if(propertyType == PropertyTypes::Table)
	{
		return reinterpret_cast<PropertyCollection*>(internalData);
	}
	else
	{
		propertyType = PropertyTypes::Table;
		return reinterpret_cast<PropertyCollection*>(internalData);
	}
}
