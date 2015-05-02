

#include <iostream>
#include "SimulationRegistry.h"
#include "ParamNames.h"
#include "ParamFileHandler.h"
#include "SimpleErosion.h"
#include "FixedTimeStepRunFallRunOff.h"
#include "TestSimulation.h"
#include "EffusionTestSimulation.h"
#include "LEM.h"
#include "Borzsony.h"
#include "Morphometry.h"
#include "SimpleRunoff.h"
#include "CompositSimulation.h"

using namespace TR;
using namespace SIMULATION;

int main( int argc, char *argv[])
{
	
	std::string paramFile;

	if (argc>1){
		paramFile = argv[1];	
	}else {
		std::cout<< "Usage: " << argv[0] << " <parameter file>" << std::endl;
		return 1;
	}
	
	SimulationRegistry registry;
	ParamFileHandler params;
	params.readParamFile( paramFile.c_str());

	try {
		
		registry.add("TestSimulation", new TestSimulation(std::cout,params));
		registry.add("SimpleErosion", new SimpleErosion(std::cout,params));
		registry.add("EffusionTestSimulation", new EffusionTestSimulation(std::cout,params));
		registry.add("FixedTimeStepRunFallRunOff", new FixedTimeStepRunFallRunOff(std::cout,params));
		registry.add("LEM", new LEM(std::cout,params));
		registry.add("borzs", new Borzsony(std::cout,params));
		registry.add("Morphometry", new Morphometry(std::cout,params));
		registry.add("SimpleRunoff", new SimpleRunoff(std::cout,params));
		registry.add("CompositSimulation", new CompositSimulation(std::cout,params));
		
		string name = params.paramAsString(SIMULATION_NAME);
		registry.run(name);
	} catch (exception e) {
		std::cout << e.what() << std::endl;
	}
}
