#ifndef LEM_H
#define LEM_H

#include "Simulation.h"

namespace SIMULATION 
{
	
class LEM : public Simulation
{
public:
	LEM(ostream & os, ParamHandler & params);
	virtual bool run();
};

};


#endif