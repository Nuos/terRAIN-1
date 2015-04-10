#include "EffusionTestSimulationParams.h"

namespace SIMULATION
{

EffusionTestSimulationParams::EffusionTestSimulationParams(const std::string & outputDirectory, std::ostream & defaultOutputStream,TR::eOutflowType outFlowType, size_t rows, size_t cols, size_t iterations)
:FixedSizeFixedTimeStepSimulationParams(outputDirectory,defaultOutputStream,outFlowType,rows, cols,iterations)
{}

}