#include "Util.h"
#include "globals.h"

using namespace std;

std::string Util::ReplaceTokensWith_particleData(std::string& text)
{
	text = ReplaceAll(text, "%data.MAX_CONNECTIONS%", to_string(particle_data_init::m_max_connections));
	text = ReplaceAll(text, "%data.EMPTY_VALUE%", to_string(particle_data_init::EMPTY_VALUE));
	auto connection_force_itterations = GetPsSetting_Int("connection_force_itterations", 1);
	text = ReplaceAll(text, "%data.ini.connection_force_itterations%", to_string(connection_force_itterations));
	return text;
}

std::string Util::ReplaceTokensWith_particleData(std::string& text, const particle_data& particleData)
{
	text = ReplaceAll(text, "%data.COUNT%", to_string(particleData.COUNT));
	text = Util::ReplaceTokensWith_particleData(text);
	return text;
}
