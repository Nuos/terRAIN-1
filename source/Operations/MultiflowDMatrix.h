#ifndef MULTIFLOWDMATRIX_H
#define MULTIFLOWDMATRIX_H

#include "ChainCodeData.h"
#include "rastermatrix.h"

namespace TR
{
	typedef ChainCodeData<double> DoubleChainCodeData;
	typedef rastermatrix<DoubleChainCodeData> MultiflowDMatrix;
}
#endif