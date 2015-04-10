#ifndef RASTERPOSITIONFUNCT_H
#define RASTERPOSITIONFUNCT_H

#include "RasterPositionFunc.h"

namespace TR
{

template <class TFunc>
class RasterPositionFuncT : public RasterPositionFunc
{
protected:
	TFunc  _Functor;
public:
	typedef TFunc typeFunctor;

	RasterPositionFuncT( RasterPositionMatrix::iterator & iOpBegin, RasterPositionMatrix::iterator & iOpEnd, DblRasterMx::iterator & iRet, TFunc functor):
		RasterPositionFunc(iOpBegin,iOpEnd,iRet), _Functor(functor)
		{}

	virtual void run();
	virtual paralellisable * clone() const;

	TFunc & getFunctor()
	{
		return _Functor;
	}
};

template<class TFunc>
void RasterPositionFuncT<TFunc>::run()
{
	RasterPositionMatrix::iterator iOp	= getOpBegin();
	RasterPositionMatrix::iterator iOpEnd = getOpEnd();
	
	DblRasterMx::iterator iRet	= getRetBegin();
	
	for (; iOp!=iOpEnd; ++iOp, ++iRet){
		_Functor.op(iOp,iRet);
	}
}

template <class TFunc>
paralellisable * RasterPositionFuncT<TFunc>:: clone() const
{
	return new RasterPositionFuncT<TFunc>(*this);
}

}

#endif