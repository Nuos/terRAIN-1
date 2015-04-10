#ifndef FIXEDTIMESTEPRUNFALLRUNOFFPARAMS_H
#define FIXEDTIMESTEPRUNFALLRUNOFFPARAMS_H

#include "FixedSizeFixedTimeStepSimulationParams.h"
namespace SIMULATION 
{

class FixedTimeStepRunFallRunOffParams : public FixedSizeFixedTimeStepSimulationParams
{
public:
	FixedTimeStepRunFallRunOffParams(const std::string & outputDirectory, std::ostream & defaultOutputStream,TR::eOutflowType outFlowType, size_t rows, size_t cols, size_t iterations);
};

}

#endif