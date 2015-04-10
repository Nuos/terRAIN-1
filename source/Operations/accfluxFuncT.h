#ifndef ACCFLUXFUNC_T
#define ACCFLUXFUNC_T

#include "rastermatrix.h"
#include "accfluxBase.h"
#include "accfluxfunctions.h"
//only for debug:
#include "OperationInterface.h"

namespace TR
{

template <class TLDD, class TFunc>
class accfluxFuncT : public accfluxBase
{
public:
	typedef typename TLDD::iterator iterator;
protected:
	typename iterator * _pLDDBegin;
	typename iterator * _pLDDEnd;
public:

	accfluxFuncT( iterator & iLDDBegin, iterator & iLDDEnd, DblRasterMx & mxDepth,DblRasterMx & mxRet)
		: accfluxBase(mxDepth,mxRet),_pLDDBegin(&iLDDBegin), _pLDDEnd(&iLDDEnd)
	{
	
	}


	virtual void run();
	virtual paralellisable * clone() const;

	iterator getLDDBegin()
	{
		return * _pLDDBegin;
	}

	iterator getLDDEnd()
	{
		return * _pLDDEnd;
	}

	void setLDDBegin(iterator & iBegin)
	{
		_pLDDBegin = &iBegin;	
	}

	void setLDDEnd(iterator & iEnd)
	{
		_pLDDEnd = &iEnd;	
	}

};

template <class TLDD, class TFunc>
void accfluxFuncT<TLDD,TFunc>::run()
{
	typename TLDD::iterator & iLDD	   = getLDDBegin();
	typename TLDD::iterator & iLDDEnd	= getLDDEnd();

	DblRasterMx & mxDepth =  getDepthMx();
	DblRasterMx & mxRet	  =  getRetMx();


	TFunc func(mxDepth,mxRet,0.0);

	int nCnt = 0;
	for (; iLDD!=iLDDEnd; ++iLDD) {	
		/*
		if (isRidgePoint(iLDD)){
			func.processPoint(iLDD);
			++nCnt;
		}
		*/
		func.processPoint(iLDD);
	}

}


template <class TLDD, class TFunc>
paralellisable * accfluxFuncT<TLDD,TFunc>::clone() const
{
	return new accfluxFuncT<TLDD,TFunc>(*_pLDDBegin,*_pLDDEnd, *_pDepthMx,*_pRetMx);
}

}
#endif