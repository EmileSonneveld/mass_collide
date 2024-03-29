#pragma once

#include <string>
#include "particle_data.h"
#include "INIReader.h"

class Util
{
public:
	Util() = default;
	virtual ~Util() = default;
	static std::string ReplaceTokensWith_particleData(std::string& text);
	static std::string ReplaceTokensWith_particleData(std::string& text, const particle_data& particleData);
};
