#include "BasicSimulationParams.h"

namespace SIMULATION
{

BasicSimulationParams::BasicSimulationParams(const std::string & outputDirectory, std::ostream & defaultOutputStream, TR::eOutflowType outFlowType):
	_outputDirectory(outputDirectory), _defaultOutputStream(defaultOutputStream), _outFlowType(outFlowType)
	{}

std::string BasicSimulationParams::getOutputDirectory() const
{
	return _outputDirectory;
}
std::ostream & BasicSimulationParams::getDefaultOutputStream()
{
	return _defaultOutputStream;
}

TR::eOutflowType BasicSimulationParams::getOutflowType() const
{
	return _outFlowType;
}



};