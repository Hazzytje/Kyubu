#ifndef __CONFIG_H__
#define __CONFIG_H__

#include <string>
#include <vector>
#include <stdlib.h>
#include <stdio.h>

namespace BromConfig {
	namespace ConfigEntryTypes {
		enum Enum{
			Bool, Int, Double, String, Table
		};
	}

	class ConfigValuePair {
	public:
		std::string Key; // name
		void* Object; // 0 false, 1 true, **** string/table/double
		ConfigEntryTypes::Enum Type; // double bool string table
		
		ConfigValuePair(std::string key, void* data, ConfigEntryTypes::Enum type);
		~ConfigValuePair(void);
	};

	class ConfigTable{
	public:
		std::vector<ConfigValuePair*> Values;

		ConfigTable(void);
		~ConfigTable(void);

		void Clear();
		bool HasValue(std::string key, ConfigEntryTypes::Enum type, int selectnum = 1);
		
		ConfigValuePair* GetPair(std::string key, int selectnum = 1);
		bool GetBool(std::string key, int selectnum = 1);
		double GetDouble(std::string key, int selectnum = 1);
		int GetInt(std::string key, int selectnum = 1);
		std::string GetString(std::string key, int selectnum = 1);
		ConfigTable* GetTable(std::string key, int selectnum = 1);

		void SetBool(std::string key, bool value, int selectnum = 1);
		void SetDouble(std::string key, double value, int selectnum = 1);
		void SetInt(std::string key, int value, int selectnum = 1);
		void SetString(std::string key, std::string value, int selectnum = 1);
		void SetTable(std::string key, ConfigTable* value, int selectnum = 1);
		
		void AddString(std::string key, std::string value);
		void AddDouble(std::string key, double value);
		void AddInt(std::string key, int value);
		void AddBool(std::string key, bool value);
		void AddTable(std::string key, ConfigTable* value);
	};

	class Config
	{

	public:
		ConfigTable Root;

		Config(void);
		~Config(void);

		bool Load(const char* file);
		bool Save(const char* file);

		void Clear();
		bool HasValue(std::string key, ConfigEntryTypes::Enum type, int selectnum = 1);
		
		void SetInt(std::string key, int value, int selectnum = 1);
		void SetBool(std::string key, bool value, int selectnum = 1);
		void SetDouble(std::string key, double value, int selectnum = 1);
		void SetString(std::string key, std::string value, int selectnum = 1);
		void SetTable(std::string key, ConfigTable* value, int selectnum = 1);
		
		ConfigValuePair* GetPair(std::string key, int selectnum = 1);
		bool GetBool(std::string key, int selectnum = 1);
		int GetInt(std::string key, int selectnum = 1);
		double GetDouble(std::string key, int selectnum = 1);
		std::string GetString(std::string key, int selectnum = 1);
		ConfigTable* GetTable(std::string key, int selectnum = 1);
		
		void AddInt(std::string key, int value);
		void AddString(std::string key, std::string value);
		void AddDouble(std::string key, double value);
		void AddBool(std::string key, bool value);
		void AddTable(std::string key, ConfigTable* value);
	private:
		void Load(FILE* fhandle, ConfigTable* master);
		void Save(int tabs, FILE* fhandle, ConfigTable* master);
	};
}

#endif