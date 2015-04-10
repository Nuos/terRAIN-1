#ifndef FIXEDTERRAINFIXEDTIMESTEPSIMULATION_H
#define FIXEDTERRAINFIXEDTIMESTEPSIMULATION_H

#include "FixedSizeTerrainSimulation.h"
#include "FixedSizeFixedTimeStepSimulationParams.h"

namespace SIMULATION 
{

class FixedTerrainFixedTimeStepSimulation : public FixedSizeTerrainSimulation
{

public:
	FixedTerrainFixedTimeStepSimulation(FixedSizeFixedTimeStepSimulationParams & params);
};

};

#endif