#ifndef UNARYMATRIXFUNCT_H
#define UNARYMATRIXFUNCT_H

#include "UnaryMatrixFunc.h"

namespace TR
{

template <class TFunc>
class UnaryMatrixFuncT: public UnaryMatrixFunc
{
protected:
	TFunc * _pFunctor;
public:
	typedef TFunc typeFunctor;
	UnaryMatrixFuncT(DblRasterMx & op, DblRasterMx & ret,TFunc & functor):UnaryMatrixFunc(op,ret), _pFunctor(&functor){} 
	virtual void run();
	virtual paralellisable * clone() const;
};

template <class TFunc>
void UnaryMatrixFuncT<TFunc>::run()
{
	_pRet->initlike(*_pOp);

	size_t nSize = _pOp->getColNr() * _pOp->getRowNr();

	double * pOpData  = _pOp->getData();
	double * pRetData = _pRet->getData();

	for (size_t i = 0; i < nSize; i++, pOpData++, pRetData++)
		*pRetData = (*_pFunctor)(*pOpData);
}

template <class TFunc>
paralellisable * UnaryMatrixFuncT<TFunc>:: clone() const
{
	return new UnaryMatrixFuncT<TFunc>(*this);
}


}

#endif