#pragma once
#include "particle_data.h"

// class children are prefixed with 'c_'
class calculate
{
public:
	virtual void process(particle_data& particle_data_ref) = 0;
    virtual void clean() = 0;
};
