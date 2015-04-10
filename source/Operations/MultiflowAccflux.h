#ifndef MULTIFLOWACCFLUX_H
#define MULTIFLOWACCFLUX_H

#include "accfluxdefs.h" 
#include <iostream>

namespace TR
{

class MultiflowAccflux : public accfluxFuncT<MultiflowDMatrix,funcAccFluxOnMLDD>
{

private:
	double _dblFluxLimitRate;
public:
	
	MultiflowAccflux( iterator & iLDDBegin, iterator & iLDDEnd, DblRasterMx & mxDepth,DblRasterMx & mxRet, double dblFluxLimitRate);
	
	virtual void run();
	virtual paralellisable * clone() const;

};


}
#endif