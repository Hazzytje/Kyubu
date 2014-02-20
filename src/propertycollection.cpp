#include "propertycollection.h"

PropertyCollection* recursiveReadProperties(Packet& pack)
{
    PropertyCollection* retVal = new PropertyCollection();
    while(true)
    {
        unsigned char valType = pack.ReadByte();
        if(valType == 0xff) return retVal;
        string keyName = pack.ReadString();
 
        switch(valType)
        {
        case PropertyTypes::Bool:
            retVal->AddBool(keyName, pack.ReadByte());
            break;
        case PropertyTypes::Int:
            retVal->AddInt(keyName, pack.ReadInt());
            break;
        case PropertyTypes::Double:
            retVal->AddDouble(keyName, pack.ReadDouble());
            printf("added %s, %f\n", keyName.c_str(), retVal->GetDouble(keyName));
            break;
        case PropertyTypes::String:
            retVal->AddString(keyName, pack.ReadString());
            break;
        case PropertyTypes::Table:
            retVal->AddTable(keyName, recursiveReadProperties(pack));
            break;
        default:
            printf("unknown enum for property type %i\n", valType);
            break;
        }
    }
 
    return retVal;
}

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
	p->internalData = reinterpret_cast<void*>(value ? 1 : 0);
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
