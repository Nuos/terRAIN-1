#ifndef UNARYWINDOWFUNCT_H
#define UNARYWINDOWFUNCT_H

#include "UnaryWindowFunc.h"

namespace TR
{

template <class TFunc>
class UnaryWindowFuncT : public UnaryWindowFunc
{
public:
	typedef TFunc typeFunctor;

protected:
	TFunc   _Func;
	size_t	_nWindowSize;
public:
	UnaryWindowFuncT( DblRasterMx::iterator & iOpBegin, DblRasterMx::iterator & iOpEnd, DblRasterMx::iterator & iRet,size_t nWinSize, TFunc & functor)
		: UnaryWindowFunc(iOpBegin,iOpEnd,iRet), _Func(functor), _nWindowSize(nWinSize)
	{}
	
	TFunc & getFunctor()
	{
		return _Func;
	}

	paralellisable * clone() const;
	void run();

	

};

template <class TFunc>
paralellisable * UnaryWindowFuncT<TFunc>::clone() const
{
	return new UnaryWindowFuncT<TFunc>(*this);
}

template <class TFunc>
void UnaryWindowFuncT<TFunc>::run()
{
	DblRasterMx::iterator & iOp		= getOpBegin();
	DblRasterMx::iterator & iOpEnd	= getOpEnd();
	DblRasterMx::iterator & iRet	= getRetIterator();
	
	TFunc & functor = getFunctor();
	
	size_t nSafeRows = iOp.getOwner().getRowNr()-_nWindowSize-1;
	size_t nSafeCols = iOp.getOwner().getColNr()-_nWindowSize-1;
	
	size_t nRow = 0;
	size_t nCol = 0;

	for (; iOp!=iOpEnd; ++iOp, ++iRet){
		
		nRow = iOp.getRow();
		nCol = iOp.getCol();

		if ( nRow<_nWindowSize || nCol<_nWindowSize || nRow>=nSafeRows || nCol>=nSafeCols )
			_Func.safeop(iOp,iRet);
		else
			_Func.op(iOp,iRet);

	}

}


}

#endif