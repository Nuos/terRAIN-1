#ifndef LONGESTFLOWPATHLENGTHFUNC_H
#define LONGESTFLOWPATHLENGTHFUNC_H

#include "rastermatrix.h"
#include "paralellisable.h"

namespace TR
{

template <class TLDD, class TFunc>
class XLongestFlowPathLengthFuncT: public paralellisable
{
public:
	typedef typename TLDD::iterator iteratorLDD;
protected:
	
	DblRasterMx * _pRet;
	iteratorLDD * _pLDDBegin;
	iteratorLDD * _pLDDEnd;

public:
	
	XLongestFlowPathLengthFuncT(iteratorLDD & iLDDBegin, iteratorLDD & iLDDEnd, DblRasterMx & mxRet):
		_pLDDBegin(&iLDDBegin),_pLDDEnd(&iLDDEnd), _pRet(&mxRet)
	{}

	iteratorLDD & getLDDBegin()
	{
		return * _pLDDBegin;
	}

	iteratorLDD & getLDDEnd()
	{
		return * _pLDDEnd;
	}
	
	DblRasterMx & getRetMx()
	{
		return * _pRet;
	}

	void setLDDBegin(iteratorLDD & iLDDBegin)
	{
		_pLDDBegin = &iLDDBegin;
	}

	void setLDDEnd(iteratorLDD & iLDDEnd)
	{
		_pLDDEnd = &iLDDEnd;
	}
	
	void setRetMx(DblRasterMx & mxRet)
	{
		_pRet = &mxRet;
	}

	virtual void run();

	virtual paralellisable * clone() const
	{
		return new XLongestFlowPathLengthFuncT<TLDD,TFunc>(*_pLDDBegin,*_pLDDEnd,*_pRet); 
	}


};

template <class TLDD, class TFunc>
void XLongestFlowPathLengthFuncT<TLDD,TFunc>::run()
{
	typename TLDD::iterator & iLDD	   = getLDDBegin();
	typename TLDD::iterator & iLDDEnd  = getLDDEnd();

	DblRasterMx & mxRet	  =  getRetMx();

	TFunc func(mxRet,iLDD.getOwner().getPixelSize());
	
	for (; iLDD!=iLDDEnd; ++iLDD) {	
		
		if (isRidgePoint(iLDD))
			func.processRidgePoint(iLDD);
	}
}

}

#endif