#ifndef SIMPLEEROSIONPARAMS_H
#define SIMPLEEROSIONPARAMS_H
#include "FixedSizeFixedTimeStepSimulationParams.h"

namespace SIMULATION 
{

class SimpleErosionParams : public FixedSizeFixedTimeStepSimulationParams
{
public:
	SimpleErosionParams(const std::string & outputDirectory, std::ostream & defaultOutputStream,TR::eOutflowType outFlowType, size_t rows, size_t cols, size_t iterations);
};

}

#endif