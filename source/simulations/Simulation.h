#ifndef SIMULATION_H
#define SIMULATION_H

#include "MultiflowDMatrix.h"
#include "rastermatrix.h"
#include "ParamHandler.h"

#include <iostream>

using namespace TR;

namespace SIMULATION
{
	
class Simulation
{
protected:
	ParamHandler & _params;
	ostream & _os;
public:
	Simulation(ostream & os, ParamHandler & params);

	virtual ~Simulation(){}
	virtual bool run()=0;
	
	
	void printmx( const DblRasterMx & mx );
	void printmx( MultiflowDMatrix & mx );

};


};

#endif