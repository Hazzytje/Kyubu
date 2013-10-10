#include "config.h"
 
 std::string toLower(std::string str){
	std::string lower = str;
    for(size_t i = 0; i < lower.size(); i++)
    {
        if(isupper(lower[i]))
        {
            lower[i] = tolower(lower[i]);
        }
    }

	return lower;
}
 
namespace BromConfig
{
bool ReadToEOL(FILE* handle)
{
    int buff;
    do
    {
        buff = fgetc(handle);
        if (buff == '\n')
            return false;
    }
    while(buff != EOF);
 
    return true;
}
 
Config::Config(void)
{
 
}
 
 
Config::~Config(void)
{
 
}
 
bool Config::Save(const char* file)
{
    FILE* handle = fopen(file, "w");
    if (!handle) return false;
 
    this->Save(0, handle, &this->Root);
 
    fclose(handle);
    return true;
}
 
void WriteTabs(int len, FILE* h)
{
    for (int i = 0; i < len; i++) fwrite("\t", 1, 1, h);
}
 
void Config::Save(int tabs, FILE* fhandle, ConfigTable* master)
{
    for (size_t i = 0; i < master->Values.size(); i++)
    {
        ConfigValuePair* p = master->Values[i];
 
        WriteTabs(tabs, fhandle);
        fwrite(p->Key.c_str(), 1, p->Key.size(), fhandle);
 
        switch(p->Type)
        {
        case ConfigEntryTypes::Bool:
            fwrite(": ", 1, 2, fhandle);
            fwrite(p->Object != nullptr ? "true" : "false", 1, p->Object != nullptr ? 4 : 5, fhandle);
            break;
 
        case ConfigEntryTypes::Double:
            fwrite(": ", 1, 2, fhandle);
 
            {
                char buff[255];
                int outputSize = sprintf(buff, "%f", *(double*)p->Object);
 
                fwrite(buff, 1, outputSize, fhandle);
            }
            fwrite("d", 1, 2, fhandle);
            break;
 
        case ConfigEntryTypes::Int:
            fwrite(": ", 1, 2, fhandle);
 
            {
                char buff[255];
                int outputSize = sprintf(buff, "%d", *(int*)p->Object);
 
                fwrite(buff, 1, outputSize, fhandle);
            }
            break;
 
        case ConfigEntryTypes::String:
            fwrite(": ", 1, 2, fhandle);
            fwrite("\"", 1, 1, fhandle);
            fwrite(((std::string*)p->Object)->c_str(), 1, ((std::string*)p->Object)->size(), fhandle);
            fwrite("\"", 1, 1, fhandle);
            break;
 
        case ConfigEntryTypes::Table:
            fwrite(" {", 1, 2, fhandle);
            fwrite("\n", 1, 1, fhandle);
            this->Save(tabs + 1, fhandle, (ConfigTable*)p->Object);
 
            WriteTabs(tabs, fhandle);
            fwrite("}", 1, 1, fhandle);
            fwrite("\n", 1, 1, fhandle);
            break;
        }
 
        fwrite("\n", 1, 1, fhandle);
    }
}
 
bool Config::Load(const char* file)
{
    FILE* handle = fopen(file, "r");
    if (!handle) return false;
 
    this->Load(handle, &this->Root);
 
    fclose(handle);
    return true;
}
 
void Config::Load(FILE* fhandle, ConfigTable* master)
{
    std::string key;
    bool instring = false;
    bool prevcomment = false;
    char tmpbuf = '\0';
 
    std::string block;
    while(true)
    {
        if (fread(&tmpbuf, 1, 1, fhandle) == 0) return;
        block += tmpbuf;
 
        if (tmpbuf != '/')
            prevcomment = false;
 
        switch(tmpbuf)
        {
        case '"':
            instring = !instring;
            break;
 
        case '/':
            if (!instring && prevcomment)
            {
                if (ReadToEOL(fhandle)) return;
 
                continue;
            }
 
            prevcomment = true;
            break;
 
        case ':':
            if (instring) break;
 
            key = block.substr(0, block.size() - 1);
 
            while(key.front() == ' ' || key.front() == '\n' || key.front() == '\t')
            {
                key.erase(key.begin());
            }
            while(key.back() == ' ' || key.back() == '\n' || key.back() == '\t')
            {
                key.erase(key.end());
            }
 
            block = "";
            break;
 
        case '{':
            if (instring) break;
 
            key = block.substr(0, block.size() - 1);
 
            while(key.front() == ' ' || key.front() == '\n' || key.front() == '\t')
            {
                key.erase(key.begin());
            }
            while(key.back() == ' ' || key.back() == '\n' || key.back() == '\t')
            {
                key.erase(key.end());
            }
 
            if (ReadToEOL(fhandle)) return;
 
            {
                ConfigTable* newtbl = new ConfigTable();
                this->Load(fhandle, newtbl);
                master->AddTable(key, newtbl);
            }
 
            key = "";
            block = "";
            break;
 
        case '}':
            if (instring) break;
            return;
 
        case '\n':
            if (key.size() == 0)
                continue;
 
            std::string value = block;
            while(key.front() == ' ' || key.front() == '\n' || key.front() == '\t')
            {
                key.erase(key.begin());
            }
            while(key.back() == ' ' || key.back() == '\n' || key.back() == '\t')
            {
                key.erase(key.end());
            }
            block = "";
 
            if (value.front() == ('\"'))
            {
                master->AddString(key, value.substr(1, value.size() - 2));
                break;
            }
 
            for(size_t i = 0; i < value.size(); i++)
            {
                if(isupper(value[i]))
                {
                    value[i] = tolower(value[i]);
                }
            }
            if (value == "true" || value == "false")
            {
                master->AddBool(key, value == "true");
                break;
            }
 
            if (value.size() > 0 && (value[0] == '-' || value[0] == '.' || (value[0] >= '0' && value[0] <= '9')))
            {
                if (value.back() == 'd')
                {
                    master->AddDouble(key.c_str(), atof(value.c_str()));
                    break;
                }
 
                if (value.find(".") != std::string::npos)
                {
                    master->AddDouble(key.c_str(), atof(value.c_str()));
                    break;
                }
 
                master->AddInt(key.c_str(), atoi(value.c_str()));
                break;
            }
            break;
        }
    }
}
 
ConfigTable::ConfigTable() {}
ConfigTable::~ConfigTable()
{
    while(this->Values.size() > 0)
    {
        delete this->Values.back();
        this->Values.pop_back();
    }
}
 
void ConfigTable::Clear()
{
    while(this->Values.size() > 0)
    {
        delete this->Values.back();
        this->Values.pop_back();
    }
}
 
void Config::AddBool(std::string key, bool value)
{
    this->Root.AddBool(key, value);
}
 
void ConfigTable::AddBool(std::string key, bool value)
{
    this->Values.push_back(new ConfigValuePair(key, reinterpret_cast<void*>((value ? 1 : 0)), ConfigEntryTypes::Bool));
}
 
void Config::AddString(std::string key, std::string value)
{
    this->Root.AddString(key, value);
}
 
void ConfigTable::AddString(std::string key, std::string value)
{
    this->Values.push_back(new ConfigValuePair(key, (void*)new std::string(value), ConfigEntryTypes::String));
}
 
void Config::AddDouble(std::string key, double value)
{
    this->Root.AddDouble(key, value);
}

void ConfigTable::AddDouble(std::string key, double value)
{
    double* num = new double;
    *num = value;
 
    this->Values.push_back(new ConfigValuePair(key, (void*)num, ConfigEntryTypes::Double));
}
 
void Config::AddInt(std::string key, int value)
{
    this->Root.AddDouble(key, value);
}

void ConfigTable::AddInt(std::string key, int value)
{
    int* num = new int;
    *num = value;
 
    this->Values.push_back(new ConfigValuePair(key, (void*)num, ConfigEntryTypes::Int));
}
 
void Config::AddTable(std::string key, ConfigTable* value)
{
    this->Root.AddTable(key, value);
}

void ConfigTable::AddTable(std::string key, ConfigTable* value)
{
    this->Values.push_back(new ConfigValuePair(key, (void*)value, ConfigEntryTypes::Table));
}
 
bool Config::HasValue(std::string key, ConfigEntryTypes::Enum type, int selectnum)
{
    return this->Root.HasValue(key, type, selectnum);
}

bool ConfigTable::HasValue(std::string key, ConfigEntryTypes::Enum type, int selectnum)
{
    key = toLower(key);
 
    int curnum = 0;
    for (size_t i = 0; i < this->Values.size(); i++)
    {
        if (this->Values[i]->Key == key && ++curnum == selectnum)
        {
            return true;
        }
    }
 
    return false;
}
 
std::string Config::GetString(std::string key, int selectnum)
{
    return this->Root.GetString(key, selectnum);
}

std::string ConfigTable::GetString(std::string key, int selectnum)
{
    key = toLower(key);
 
    int curnum = 0;
    for (size_t i = 0; i < this->Values.size(); i++)
    {
        if (this->Values[i]->Key == key && this->Values[i]->Type == ConfigEntryTypes::String && ++curnum == selectnum)
        {
			return *((std::string*)this->Values[i]->Object);
        }
    }
 
    return "";
}
 
void Config::SetString(std::string key, std::string value, int selectnum)
{
    this->Root.SetString(key, value, selectnum);
}
 
void ConfigTable::SetString(std::string key, std::string value, int selectnum)
{
    key = toLower(key);
 
    int curnum = 0;
    for (size_t i = 0; i < this->Values.size(); i++)
    {
        if (this->Values[i]->Key == key && this->Values[i]->Type == ConfigEntryTypes::String && ++curnum == selectnum)
        {
            delete this->Values[i];
            this->Values[i] = new ConfigValuePair(toLower(key), (void*)new std::string(value), ConfigEntryTypes::String);
            return;
        }
    }
 
    this->Values.push_back(new ConfigValuePair(toLower(key), (void*)new std::string(value), ConfigEntryTypes::String));
    return;
}
 
ConfigValuePair* Config::GetPair(std::string key, int selectnum)
{
    return this->Root.GetPair(key, selectnum);
}
 
ConfigValuePair* ConfigTable::GetPair(std::string key, int selectnum)
{
    key = toLower(key);
 
    int curnum = 0;
    for (size_t i = 0; i < this->Values.size(); i++)
    {
        if (this->Values[i]->Key == key && ++curnum == selectnum)
        {
            return this->Values[i];
        }
    }
 
    return nullptr;
}
 
bool Config::GetBool(std::string key, int selectnum)
{
    return this->Root.GetBool(key, selectnum);
}
 
bool ConfigTable::GetBool(std::string key, int selectnum)
{
    key = toLower(key);
 
    int curnum = 0;
    for (size_t i = 0; i < this->Values.size(); i++)
    {
        if (this->Values[i]->Key == key && this->Values[i]->Type == ConfigEntryTypes::Bool && ++curnum == selectnum)
        {
            return this->Values[i]->Object != 0;
        }
    }
 
    return false;
}
 
void Config::SetBool(std::string key, bool value, int selectnum)
{
    this->Root.SetBool(key, value, selectnum);
}
 
void ConfigTable::SetBool(std::string key, bool value, int selectnum)
{
    key = toLower(key);
 
    int curnum = 0;
    for (size_t i = 0; i < this->Values.size(); i++)
    {
        if (this->Values[i]->Key == key && this->Values[i]->Type == ConfigEntryTypes::Bool && ++curnum == selectnum)
        {
            delete this->Values[i];
            this->Values[i] = new ConfigValuePair(toLower(key), reinterpret_cast<void*>(value ? 1 : 0), ConfigEntryTypes::Bool);
            return;
        }
    }
 
    this->Values.push_back(new ConfigValuePair(toLower(key), reinterpret_cast<void*>(value ? 1 : 0), ConfigEntryTypes::Bool));
    return;
}
 
double Config::GetDouble(std::string key, int selectnum)
{
    return this->Root.GetDouble(key, selectnum);
}

double ConfigTable::GetDouble(std::string key, int selectnum)
{
    key = toLower(key);
 
    int curnum = 0;
    for (size_t i = 0; i < this->Values.size(); i++)
    {
        if (this->Values[i]->Key == key && this->Values[i]->Type == ConfigEntryTypes::Double && ++curnum == selectnum)
        {
            return *(double*)this->Values[i]->Object;
        }
    }
 
    return 0;
}
 
int Config::GetInt(std::string key, int selectnum)
{
    return this->Root.GetInt(key, selectnum);
}

int ConfigTable::GetInt(std::string key, int selectnum)
{
    key = toLower(key);
 
    int curnum = 0;
    for (size_t i = 0; i < this->Values.size(); i++)
    {
        if (this->Values[i]->Key == key && this->Values[i]->Type == ConfigEntryTypes::Int && ++curnum == selectnum)
        {
            return *(int*)this->Values[i]->Object;
        }
    }
 
    return 0;
}
 
void Config::SetDouble(std::string key, double value, int selectnum)
{
    this->Root.SetDouble(key, value, selectnum);
}

void ConfigTable::SetDouble(std::string key, double value, int selectnum)
{
    key = toLower(key);
    double* num = new double;
    *num = value;
 
    int curnum = 0;
    for (size_t i = 0; i < this->Values.size(); i++)
    {
        if (this->Values[i]->Key == key && this->Values[i]->Type == ConfigEntryTypes::Double && ++curnum == selectnum)
        {
            delete this->Values[i];
            this->Values[i] = new ConfigValuePair(toLower(key), (void*)num, ConfigEntryTypes::Double);
            return;
        }
    }
 
    this->Values.push_back(new ConfigValuePair(toLower(key), (void*)num, ConfigEntryTypes::Double));
    return;
}
 
void Config::SetInt(std::string key, int value, int selectnum)
{
    this->Root.SetInt(key, value, selectnum);
}

void ConfigTable::SetInt(std::string key, int value, int selectnum)
{
    key = toLower(key);
    double* num = new double;
    *num = value;
 
    int curnum = 0;
    for (size_t i = 0; i < this->Values.size(); i++)
    {
        if (this->Values[i]->Key == key && this->Values[i]->Type == ConfigEntryTypes::Int && ++curnum == selectnum)
        {
            delete this->Values[i];
            this->Values[i] = new ConfigValuePair(toLower(key), (void*)num, ConfigEntryTypes::Int);
            return;
        }
    }
 
    this->Values.push_back(new ConfigValuePair(toLower(key), (void*)num, ConfigEntryTypes::Int));
    return;
}
 
ConfigTable* Config::GetTable(std::string key, int selectnum)
{
    return this->Root.GetTable(key, selectnum);
}

ConfigTable* ConfigTable::GetTable(std::string key, int selectnum)
{
    key = toLower(key);
 
    int curnum = 0;
    for (size_t i = 0; i < this->Values.size(); i++)
    {
        if (this->Values[i]->Key == key && this->Values[i]->Type == ConfigEntryTypes::Table && ++curnum == selectnum)
        {
            return (ConfigTable*)this->Values[i]->Object;
        }
    }
 
    return nullptr;
}
 
void Config::SetTable(std::string key, ConfigTable* value, int selectnum)
{
    this->Root.SetTable(key, value, selectnum);
}

void ConfigTable::SetTable(std::string key, ConfigTable* value, int selectnum)
{
    key = toLower(key);
 
    int curnum = 0;
    for (size_t i = 0; i < this->Values.size(); i++)
    {
        if (this->Values[i]->Key == key && this->Values[i]->Type == ConfigEntryTypes::Table && ++curnum == selectnum)
        {
            delete this->Values[i];
            this->Values[i] = new ConfigValuePair(toLower(key), (void*)value, ConfigEntryTypes::Table);
            return;
        }
    }
 
    this->Values.push_back(new ConfigValuePair(toLower(key), (void*)value, ConfigEntryTypes::Table));
    return;
}


 
ConfigValuePair::ConfigValuePair(std::string key, void* obj, ConfigEntryTypes::Enum type)
{
    std::string lower = toLower(key);
    this->Key = key;
    this->Object = obj;
    this->Type = type;
}
 
ConfigValuePair::~ConfigValuePair()
{
    if (this->Object == nullptr)
        return;
 
    if (this->Type == ConfigEntryTypes::String)
        delete (std::string*)this->Object;
 
    if (this->Type == ConfigEntryTypes::Double)
        delete (double*)this->Object;
 
    if (this->Type == ConfigEntryTypes::Int)
        delete (int*)this->Object;
 
    if (this->Type == ConfigEntryTypes::Table)
        delete (ConfigTable*)this->Object;
}
}
 
 