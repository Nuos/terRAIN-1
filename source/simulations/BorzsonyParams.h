#ifndef BORZSONYPARAMS_H
#define BORZSONYPARAMS_H

#include "FixedSizeFixedTimeStepSimulationParams.h"

namespace SIMULATION 
{

class BorzsonyParams : public FixedSizeFixedTimeStepSimulationParams
{
public:
	BorzsonyParams(const std::string & outputDirectory, std::ostream & defaultOutputStream,TR::eOutflowType outFlowType, size_t rows, size_t cols, size_t iterations);
};

}


#endif