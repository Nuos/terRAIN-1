#ifndef SIMULATIONPARAMS_H
#define SIMULATIONPARAMS_H

#include <string>
#include <iostream>
#include "defs.h"

namespace SIMULATION
{

class BasicSimulationParams
{

protected:
	std::string  _outputDirectory;
	std::ostream & _defaultOutputStream;
	TR::eOutflowType _outFlowType;

public:

	BasicSimulationParams(const std::string & outputDirectory, std::ostream & defaultOutputStream, TR::eOutflowType outFlowType);

	std::string getOutputDirectory() const;
	std::ostream & getDefaultOutputStream();
	TR::eOutflowType getOutflowType() const;
};


};

#endif