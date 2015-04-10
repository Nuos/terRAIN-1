#include "FixedSizeFixedTimeStepSimulationParams.h"

namespace SIMULATION
{

FixedSizeFixedTimeStepSimulationParams::FixedSizeFixedTimeStepSimulationParams(const std::string & outputDirectory, std::ostream & defaultOutputStream,TR::eOutflowType outFlowType, size_t rows, size_t cols, size_t iterations):
FixedSizeTerrainSimulationParams(outputDirectory, defaultOutputStream,outFlowType, rows,cols),_iterations(iterations)
{}

size_t FixedSizeFixedTimeStepSimulationParams::getIterations() const
{
	return _iterations;
}

}