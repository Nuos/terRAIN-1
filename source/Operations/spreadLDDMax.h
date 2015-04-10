#ifndef SPREADLDDMAX_H
#define SPREADLDDMAX_H

#include "rastermatrix.h"
#include "MultiflowDMatrix.h"

namespace TR {
class SpreadLDDMax 
{
protected:
	const MultiflowDMatrix & _mxMLDD;
	const DblRasterMx & _mxPoints;
    DblRasterMx & _mxRet;
	double _pixelSize;
	double _dblFluxLimitRate;
public:

	SpreadLDDMax(const MultiflowDMatrix & mxMLDD, const DblRasterMx & mxPoints,DblRasterMx & mxRet, double dblFluxLimitRate);
    void run();
protected:
	void processRidgePoint( MultiflowDMatrix::iterator & item);
};
}

#endif