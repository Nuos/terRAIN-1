#ifndef FIXEDTIMESTEPRUNFALLRUNOFF_H
#define FIXEDTIMESTEPRUNFALLRUNOFF_H

#include "Simulation.h"

namespace SIMULATION 
{
	
class FixedTimeStepRunFallRunOff : public Simulation
{
public:
	FixedTimeStepRunFallRunOff(ostream & os, ParamHandler & params);
	virtual bool run();
};

};

#endif