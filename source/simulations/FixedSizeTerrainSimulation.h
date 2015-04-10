#ifndef FIXEDSIZETERRAINSIMULATION_H
#define FIXEDSIZETERRAINSIMULATION_H

#include "Simulation.h"
#include "FixedSizeTerrainSimulationParams.h"

namespace SIMULATION 
{

class FixedSizeTerrainSimulation : public Simulation
{
public:
	
	FixedSizeTerrainSimulation(const FixedSizeTerrainSimulationParams & params);	
};

};

#endif