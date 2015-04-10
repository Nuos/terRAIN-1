#ifndef BINARYWINDOWFUNCT_H
#define BINARYWINDOWFUNCT_H

#include "BinaryWindowFunc.h"

namespace TR
{

template<class TFunc>
class BinaryWindowFuncT : public BinaryWindowFunc
{
protected:
	TFunc  _Functor;
	size_t _nWindowSize;
public:
	typedef TFunc typeFunctor;

	BinaryWindowFuncT( DblRasterMx::iterator & iOp1Begin, DblRasterMx::iterator & iOp1End, DblRasterMx::iterator & iOp2Begin, DblRasterMx::iterator & iRet, size_t nWinSize, TFunc & functor) : _Functor(functor),BinaryWindowFunc( iOp1Begin, iOp1End, iOp2Begin,iRet),_nWindowSize(nWinSize){}

	virtual void run();
	virtual paralellisable * clone() const;

	TFunc & getFunctor()
	{
		return _Functor;
	}

};

template<class TFunc>
void BinaryWindowFuncT<TFunc>::run()
{
	DblRasterMx::iterator & iOp1	= getOp1Begin();
	DblRasterMx::iterator & iOp1End	= getOp1End();
	DblRasterMx::iterator & iOp2	= getOp2Begin();
	DblRasterMx::iterator & iRet	= getRetIterator();
	
	TFunc & functor = getFunctor();
	
	size_t nSafeRows = iOp1.getOwner().getRowNr()-_nWindowSize-1;
	size_t nSafeCols = iOp1.getOwner().getColNr()-_nWindowSize-1;
	
	size_t nRow = 0;
	size_t nCol = 0;

	for (; iOp1!=iOp1End; ++iOp1,++iOp2, ++iRet){
		
		nRow = iOp1.getRow();
		nCol = iOp1.getCol();

		if ( nRow<_nWindowSize || nCol<_nWindowSize || nRow>=nSafeRows || nCol>=nSafeCols )
			_Functor.safeop(iOp1,iOp2,iRet);
		else
			_Functor.op(iOp1,iOp2,iRet);

	}
}

template<class TFunc>
paralellisable * BinaryWindowFuncT<TFunc>::clone() const
{
	return new BinaryWindowFuncT<TFunc>(*this);
}



}
#endif