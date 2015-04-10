#ifndef SIMPLEMATRIXFUNC_H
#define SIMPLEMATRIXFUNC_H

#include "MatrixFunc.h"

namespace TR
{

class SimpleMatrixFunc : public MatrixFunc
{
public:
	SimpleMatrixFunc(DblRasterMx & ret):MatrixFunc(ret){}
};

template<class TFunc>
class SimpleMatrixFuncT : public SimpleMatrixFunc
{
protected:
	TFunc * _pFunctor;
public:
	typedef TFunc typeFunctor;
	SimpleMatrixFuncT(DblRasterMx & ret, TFunc & functor):SimpleMatrixFunc(ret),_pFunctor(&functor) {} 
	
	virtual void run();
	virtual paralellisable * clone() const;
	
};

template<class TFunc>
void SimpleMatrixFuncT<TFunc>::run()
{
	size_t nSize = _pRet->getColNr() * _pRet->getRowNr();

	double * pRetData = _pRet->getData();

	for (size_t i = 0; i < nSize; i++, pRetData++)
		(*_pFunctor)(*pRetData);
}

template<class TFunc>
paralellisable * SimpleMatrixFuncT<TFunc>::clone() const 
{
	return new SimpleMatrixFuncT<TFunc>(*this);
}


}


#endif