#include "MultiflowAccflux.h"

namespace TR
{

MultiflowAccflux::MultiflowAccflux( iterator & iLDDBegin, iterator & iLDDEnd, DblRasterMx & mxDepth,DblRasterMx & mxRet, double dblFluxLimitRate)
	: accfluxFuncT<MultiflowDMatrix,funcAccFluxOnMLDD>(iLDDBegin,iLDDEnd,mxDepth,mxRet),_dblFluxLimitRate(dblFluxLimitRate)
{

}

void MultiflowAccflux::run()
{
	
	MultiflowDMatrix::iterator & iLDD	   = getLDDBegin();
	MultiflowDMatrix::iterator & iLDDEnd   = getLDDEnd();

	DblRasterMx & mxDepth =  getDepthMx();
	DblRasterMx & mxRet	  =  getRetMx();

	funcAccFluxOnMLDD func(mxDepth,mxRet,_dblFluxLimitRate);

	for (; iLDD!=iLDDEnd; ++iLDD)	
		func.processPoint(iLDD);
	
}


paralellisable * MultiflowAccflux::clone() const
{
	return new MultiflowAccflux(*_pLDDBegin,*_pLDDEnd, *_pDepthMx,*_pRetMx,_dblFluxLimitRate);
}




}