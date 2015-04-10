#ifndef FIXEDSIZETERRAINSIMULATIONPARAMS_H
#define FIXEDSIZETERRAINSIMULATIONPARAMS_H

#include "BasicSimulationParams.h"

namespace SIMULATION
{

class FixedSizeTerrainSimulationParams: public BasicSimulationParams
{
protected:
	size_t _rows;
	size_t _cols;

public:
	FixedSizeTerrainSimulationParams(const std::string & outputDirectory, std::ostream & defaultOutputStream,TR::eOutflowType outFlowType, size_t rows, size_t cols);
	
	size_t getCols() const;
	size_t getRows() const;
};	

};


#endif