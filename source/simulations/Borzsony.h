#ifndef BORZSONY_H
#define BORZSONY_H

#include "FixedTerrainFixedTimeStepSimulation.h"

namespace SIMULATION 
{
	
class Borzsony : public Simulation
{
public:
	Borzsony(ostream & os, ParamHandler & params);
	virtual bool run();
};

};


#endif