#ifndef EFFUSIONTESTSIMULATIN_H
#define EFFUSIONTESTSIMULATIN_H

#include "Simulation.h"

namespace SIMULATION 
{

class EffusionTestSimulation : public Simulation
{
public:
	EffusionTestSimulation(ostream & os, ParamHandler & params);
	virtual bool run();
};

}


#endif