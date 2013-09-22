#include "propertycollection.h"

PropertyCollection::PropertyCollection()
{
	//ctor
}

PropertyCollection::~PropertyCollection()
{
	for (auto p : properties){
		delete p.second;
	}
}

using std::string;

void PropertyCollection::AddInt(string key, int value)
{
	PropertyPair* p = new PropertyPair();
	p->internalData = new int(value);
	properties[key] = p;
}

void PropertyCollection::AddBool(string key, bool value)
{
	PropertyPair* p = new PropertyPair();
	p->internalData = (void*)(value ? 1 : 0);
	properties[key] = p;
}

void PropertyCollection::AddDouble(string key, double value)
{
	PropertyPair* p = new PropertyPair();
	p->internalData = new double(value);
	properties[key] = p;
}

void PropertyCollection::AddTable(string key, PropertyCollection* value)
{
	PropertyPair* p = new PropertyPair();
	p->internalData = value;
	properties.insert(make_pair(key, p));
}

void PropertyCollection::AddString(string key, string value)
{
	PropertyPair* p = new PropertyPair();
	p->internalData = new string(value);
	properties[key] = p;
}


int PropertyCollection::GetInt(string key)
{
	return *properties[key]->getAsInt();
}

bool PropertyCollection::GetBool(string key)
{
	return properties[key]->getAsBool() != NULL;
}

double PropertyCollection::GetDouble(string key)
{
	PropertyPair* p = properties[key];
	double num = *(p->getAsDouble());
	return num;
}

PropertyCollection* PropertyCollection::GetTable(string key)
{
	return properties[key]->getAsTable();
}

string PropertyCollection::GetString(string key)
{
	return *properties[key]->getAsString();
}
