#ifndef EFFUSIONTESTSIMULATIONPARAMS_H
#define EFFUSIONTESTSIMULATIONPARAMS_H

#include "FixedSizeFixedTimeStepSimulationParams.h"

namespace SIMULATION 
{

class EffusionTestSimulationParams : public FixedSizeFixedTimeStepSimulationParams
{
public:
	EffusionTestSimulationParams(const std::string & outputDirectory, std::ostream & defaultOutputStream,TR::eOutflowType outFlowType, size_t rows, size_t cols, size_t iterations);

};


}
#endif