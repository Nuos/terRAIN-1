#ifndef SIMPLEROSION_H
#define SIMPLEROSION_H

#include "Simulation.h"

namespace SIMULATION 
{
	
class SimpleErosion : public Simulation
{
public:
	SimpleErosion(ostream & os, ParamHandler & params);
	virtual bool run();
};

};


#endif