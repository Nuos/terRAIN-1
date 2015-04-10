#ifndef UNARYMFLDDFUNCT_H
#define UNARYMFLDDFUNCT_H

#include "UnaryMLDDFunc.h"
#include "MultiflowDMatrix.h"
#include "rastermatrix.h"

namespace TR {

template<class TFunc>
class UnaryMLDDFuncT : public UnaryMLDDFunc
{
private:
	TFunc  _Functor;
public:
	
	typedef TFunc typeFunctor;
	
	UnaryMLDDFuncT(MultiflowDMatrix::iterator & iOpBegin, MultiflowDMatrix::iterator & iOpEnd, DblRasterMx::iterator & iRet, TFunc & functor) 
		: UnaryMLDDFunc(iOpBegin,iOpEnd,iRet), _Functor(functor)
	{}

	virtual void run();
	virtual paralellisable * clone() const;

	TFunc & getFunctor()
	{
		return _Functor;
	}

};

template<class TFunc>
void UnaryMLDDFuncT<TFunc>::run()
{
	TFunc & functor = getFunctor();
	
	MultiflowDMatrix::iterator iter = getOpBegin();
	MultiflowDMatrix::iterator end = getOpEnd();
	DblRasterMx::iterator iRet = getRetIterator();

	for (;iter!=end; ++iter,++iRet) {
		*iRet = functor(iter);
	}
}

template<class TFunc>
paralellisable * UnaryMLDDFuncT<TFunc>::clone() const
{
	return new UnaryMLDDFuncT<TFunc>(*this);
}

}


#endif