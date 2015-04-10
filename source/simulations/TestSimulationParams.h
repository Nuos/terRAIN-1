#ifndef TESTSIMULATIONPARAMS_H
#define TESTSIMULATIONPARAMS_H

#include "FixedSizeFixedTimeStepSimulationParams.h"

namespace SIMULATION
{

class TestSimulationParams : public FixedSizeFixedTimeStepSimulationParams
{
public:
	TestSimulationParams(const std::string & outputDirectory, std::ostream & defaultOutputStream,TR::eOutflowType outFlowType, size_t rows, size_t cols, size_t iterations);

};

}

#endif