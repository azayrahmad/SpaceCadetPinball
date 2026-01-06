#pragma once
#include <map>

class IniFile
{
public:
	explicit IniFile(const std::string& fileName);
	~IniFile();
	int get_int(const std::string& key, int defaultValue);
	std::string get_string(const std::string& key, const std::string& defaultValue);
	void set_int(const std::string& key, int value);
	void set_string(const std::string& key, const std::string& value);

private:
	std::string GetSetting(const std::string& key, const std::string& value);
	void SetSetting(const std::string& key, const std::string& value);

	std::string FileName;
	std::map<std::string, std::string> Settings;
	bool Dirty = false;
};
