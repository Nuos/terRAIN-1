#ifndef TESTSIMULATION_H
#define TESTSIMULATION_H

#include "Simulation.h"

namespace SIMULATION
{

class TestSimulation : public Simulation
{
public:
	TestSimulation(ostream & os, ParamHandler & params);
	virtual bool run();
};

}

#endif