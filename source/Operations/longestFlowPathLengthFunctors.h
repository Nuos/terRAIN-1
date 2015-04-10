#ifndef LONGESTFLOWPATHLENGTHFUNCTORS_H
#define LONGESTFLOWPATHLENGTHFUNCTORS_H

#include "MultiflowDMatrix.h"

using namespace TR;

namespace FUNC
{

class funcLongestFlowPathLengtBase
{
protected:
	DblRasterMx & _mxRet;
	double _dblPixelSize;
	size_t _nMaxIter;
public:
	funcLongestFlowPathLengtBase( DblRasterMx & mxRet, double dblPixelSize ):_mxRet(mxRet), _dblPixelSize(dblPixelSize)
	{
		_nMaxIter = mxRet.getColNr()*mxRet.getRowNr();
	}
};

class funcLongestFlowPathLengthLDD : public funcLongestFlowPathLengtBase
{
	
public:
	funcLongestFlowPathLengthLDD( DblRasterMx & mxRet,double dblPixelSize  ):funcLongestFlowPathLengtBase(mxRet,dblPixelSize)
	{}

	void processRidgePoint( DblRasterMx::iterator & iLDD);
};

class funcLongestFlowPathLengthMLDD : public funcLongestFlowPathLengtBase
{
public:

	funcLongestFlowPathLengthMLDD( DblRasterMx & mxRet,double dblPixelSize ):funcLongestFlowPathLengtBase(mxRet,dblPixelSize)
	{}

	void processRidgePoint( MultiflowDMatrix::iterator & iLDD );
};

}
#endif