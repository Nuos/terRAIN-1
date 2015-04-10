#ifndef MORPHOMTRY_H
#define MORPHOMTRY_H

#include "Simulation.h"

namespace SIMULATION 
{
	
class Morphometry : public Simulation
{
public:
	Morphometry(ostream & os, ParamHandler & params);
	virtual bool run();
};

};


#endif