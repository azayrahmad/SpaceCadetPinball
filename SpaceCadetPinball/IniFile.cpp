#include "pch.h"
#include "IniFile.h"

#include <fstream>
#include <sstream>

IniFile::IniFile(const std::string& fileName) : FileName(fileName)
{
	std::ifstream file(FileName);
	if (file.is_open())
	{
		std::string line;
		while (std::getline(file, line))
		{
			std::stringstream ss(line);
			std::string key, value;
			if (std::getline(ss, key, '=') && std::getline(ss, value))
			{
				Settings[key] = value;
			}
		}
	}
}

IniFile::~IniFile()
{
#ifndef __EMSCRIPTEN__
	if (Dirty)
	{
		std::ofstream file(FileName);
		if (file.is_open())
		{
			for (const auto& setting : Settings)
			{
				file << setting.first << "=" << setting.second << std::endl;
			}
		}
	}
#endif
}

int IniFile::get_int(const std::string& key, int defaultValue)
{
	auto value = GetSetting(key, std::to_string(defaultValue));
	return std::stoi(value);
}

std::string IniFile::get_string(const std::string& key, const std::string& defaultValue)
{
	return GetSetting(key, defaultValue);
}

void IniFile::set_int(const std::string& key, int value)
{
	SetSetting(key, std::to_string(value));
}

void IniFile::set_string(const std::string& key, const std::string& value)
{
	SetSetting(key, value);
}

std::string IniFile::GetSetting(const std::string& key, const std::string& value)
{
	auto setting = Settings.find(key);
	if (setting == Settings.end())
	{
		Settings[key] = value;
		Dirty = true;
		return value;
	}
	return setting->second;
}

void IniFile::SetSetting(const std::string& key, const std::string& value)
{
	Settings[key] = value;
	Dirty = true;

#ifdef __EMSCRIPTEN__
	std::ofstream file(FileName);
	if (file.is_open())
	{
		for (const auto& setting : Settings)
		{
			file << setting.first << "=" << setting.second << std::endl;
		}
	}
#endif
}
