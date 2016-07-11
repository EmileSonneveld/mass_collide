#pragma once

#include <string>
#include "particle_data.h"
#include "INIReader.h"

class Util
{
public:
	Util();
	virtual ~Util();
	static std::string ReplaceTokensWith_particleData(std::string& text, const particle_data& particleData);
	//static std::string ReplaceTokensWith_ini(std::string& text, INIReader& data);
};
