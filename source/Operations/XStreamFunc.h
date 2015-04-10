#ifndef XSTREAMFUNC_H
#define XSTREAMFUNC_H

#include "paralellisable.h"
#include "rastermatrix.h"
#include "RasterPositionMatrix.h"

namespace TR
{

template <class TLDD>
class XStreamFunc: public paralellisable
{

public:
	typedef typename TLDD::iterator iteratorLDD;
protected:

	iteratorLDD	* _pLDDBegin;
	iteratorLDD * _pLDDEnd;
	DblRasterMx::iterator * _pDepth;
	DblRasterMx::iterator * _pRet;
	RasterPositionMatrix::iterator * _posMatrix;

public:
	XStreamFunc(iteratorLDD & iLDDBegin, iteratorLDD & iLDDEnd, DblRasterMx::iterator & iDepth, DblRasterMx::iterator & iRet, RasterPositionMatrix::iterator & iPosMatrix):
		_pLDDBegin(&iLDDBegin), _pLDDEnd(&iLDDEnd), _pDepth(&iDepth), _pRet(&iRet), _posMatrix(&iPosMatrix)
		{}

	void setLDDBegin( iteratorLDD & iLDDBegin )
	{
		_pLDDBegin = &iLDDBegin;	
	}

	void setLDDEnd( iteratorLDD & iLDDEnd )
	{
		_pLDDEnd = &iLDDEnd;
	}

	void setDepth( DblRasterMx::iterator & iDepth )
	{
		_pDepth = &iDepth;
	}

	void setRet( DblRasterMx::iterator & iRet )
	{
		_pRet = &iRet;
	}

	iteratorLDD & getLDDBegin()
	{
		return *_pLDDBegin;	
	}

	iteratorLDD & getLDDEnd()
	{
		return *_pLDDEnd;
	}

	DblRasterMx::iterator & getDepth()
	{
		return * _pDepth;
	}

	DblRasterMx::iterator & getRet()
	{
		return * _pRet;
	}

	RasterPositionMatrix::iterator & getRasterPosMatrixIterator()
	{
		return * _posMatrix;
	}
};

}
#endif