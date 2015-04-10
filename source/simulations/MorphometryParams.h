#ifndef MORPHOMETRYPARAMS_H
#define MORPHOMETRYPARAMS_H

#include "FixedSizeFixedTimeStepSimulationParams.h"

namespace SIMULATION 
{

class MorphometryParams : public FixedSizeFixedTimeStepSimulationParams
{
public:
	MorphometryParams(const std::string & outputDirectory, std::ostream & defaultOutputStream,TR::eOutflowType outFlowType, size_t rows, size_t cols, size_t iterations);
};

}


#endif