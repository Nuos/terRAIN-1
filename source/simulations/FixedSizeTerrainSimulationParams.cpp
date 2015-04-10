#include "FixedSizeTerrainSimulationParams.h"

namespace SIMULATION
{

FixedSizeTerrainSimulationParams::FixedSizeTerrainSimulationParams(const std::string & outputDirectory, std::ostream & defaultOutputStream,TR::eOutflowType outFlowType, size_t rows, size_t cols)
:BasicSimulationParams(outputDirectory,defaultOutputStream,outFlowType), _rows(rows),_cols(cols)
{}

size_t FixedSizeTerrainSimulationParams::getCols() const
{
	return _cols;
}

size_t FixedSizeTerrainSimulationParams::getRows() const
{
	return _rows;
}

};