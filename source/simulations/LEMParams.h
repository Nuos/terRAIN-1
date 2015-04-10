#ifndef LEMPARAMS_H
#define LEMPARAMS_H

#include "FixedSizeFixedTimeStepSimulationParams.h"

namespace SIMULATION 
{

class LEMParams : public FixedSizeFixedTimeStepSimulationParams
{
public:
	LEMParams(const std::string & outputDirectory, std::ostream & defaultOutputStream,TR::eOutflowType outFlowType, size_t rows, size_t cols, size_t iterations);
};

}


#endif