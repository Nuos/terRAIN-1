#ifndef XSTREAMFUNCT_H
#define XSTREAMFUNCT_H

#include "XStreamFunc.h"

namespace TR
{

template <class TLDD, class TFunc>
class XStreamFuncT : public XStreamFunc<TLDD>
{
protected:
	TFunc * _pFunctor;
public:
	typedef TFunc typeFunctor;

	XStreamFuncT(iteratorLDD & iLDDBegin, iteratorLDD & iLDDEnd, DblRasterMx::iterator & iDepth, DblRasterMx::iterator & iRet, RasterPositionMatrix::iterator & iPosMatrix, TFunc & functor):
		XStreamFunc<TLDD>(iLDDBegin, iLDDEnd, iDepth, iRet, iPosMatrix ), _pFunctor(&functor)
		{}

	virtual void run();
	virtual paralellisable * clone() const;
};

template <class TLDD, class TFunc>
void XStreamFuncT<TLDD,TFunc>::run()
{
	iteratorLDD & iLDD			 = getLDDBegin();
	iteratorLDD & iLDDEnd		 = getLDDEnd();
	DblRasterMx::iterator iDepth = getDepth();
	DblRasterMx::iterator iRet	 = getRet();
	
	RasterPositionMatrix::iterator iRasterPosMx = getRasterPosMatrixIterator();
	
	size_t nLastRowIndex = iLDD.getMatrixRowNr()-1;
	size_t nLastColIndex = iLDD.getMatrixColNr()-1;
	
	size_t nRow = 0;
	size_t nCol = 0;

	for (;iLDD!=iLDDEnd; ++iLDD, ++iDepth, ++iRet, ++iRasterPosMx) {
		
		nRow = iLDD.getRow();
		nCol = iLDD.getCol();

		if ( nRow==0 || nCol==0 || nRow==nLastRowIndex || nCol==nLastColIndex )
			*iRet = _pFunctor->safeop(iLDD,iDepth,iRasterPosMx);
		else
			*iRet = _pFunctor->op(iLDD,iDepth,iRasterPosMx);	
	}
}

template <class TLDD, class TFunc>
paralellisable * XStreamFuncT<TLDD,TFunc>::clone() const
{
	return new XStreamFuncT<TLDD,TFunc>(*_pLDDBegin, *_pLDDEnd, *_pDepth, *_pRet,*_posMatrix, *_pFunctor);
}

}

#endif