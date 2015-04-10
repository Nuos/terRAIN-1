#ifndef FIXEDSIZEFIXEDTIMESTEPSIMULATIONPARAMS_H
#define FIXEDSIZEFIXEDTIMESTEPSIMULATIONPARAMS_H

#include "FixedSizeTerrainSimulationParams.h"

namespace SIMULATION
{

class FixedSizeFixedTimeStepSimulationParams : public FixedSizeTerrainSimulationParams
{
protected:
	size_t _iterations;
public:
	FixedSizeFixedTimeStepSimulationParams(const std::string & outputDirectory, std::ostream & defaultOutputStream,TR::eOutflowType outFlowType, size_t rows, size_t cols, size_t iterations);
	size_t getIterations() const;
};

};

#endif