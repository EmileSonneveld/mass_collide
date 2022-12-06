#include "Util.h"
#include "globals.h"

using namespace std;

std::string Util::ReplaceTokensWith_particleData(std::string & text, const particle_data & particleData)
{
	text = ReplaceAll(text, "%data.MAX_CONNECTIONS%", to_string(particle_data_init::m_max_connections));
	text = ReplaceAll(text, "%data.EMPTY_VALUE%", to_string(particle_data_init::EMPTY_VALUE));
	text = ReplaceAll(text, "%data.COUNT%", to_string(particleData.COUNT));
	return text;
}
