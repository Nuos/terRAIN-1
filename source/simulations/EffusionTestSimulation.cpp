#include "EffusionTestSimulation.h"
#include "OperationInterface.h"
#include "StatFile.h"

using namespace TR;

namespace SIMULATION 
{

EffusionTestSimulation::EffusionTestSimulation(ostream & os, ParamHandler & params):
	Simulation(os,params)
{}

	
bool EffusionTestSimulation::run()
{	
	size_t rows = 100;
	size_t cols = 100;
	size_t iterationNr = 100;
	size_t rainSteps = 20;
	
	setOutflowType(ofBottomLeft);

	double baseHeight = 100.0;


	DblRasterMx mxBase;
	DblRasterMx mxTerrain;
	DblRasterMx mxRandom;
	

	mapattr(rows,cols,1.0,baseHeight,mxBase);
	mapattr(rows,cols,1.0,0.0,mxTerrain);
	mapattr(rows,cols,1.0,0.0,mxRandom);


	uniform(mxRandom);
	add(mxBase,mxRandom,mxTerrain);

	int stepPerIteration = 10;


	return true;
}

}