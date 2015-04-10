#ifndef BINARYMATRIXFUNCT_H
#define BINARYMATRIXFUNCT_H

#include "BinaryMatrixFunc.h"
#include "exOperandsNotCompatibile.h"

namespace TR
{

template<class TFunc>
class BinaryMatrixFuncT: public BinaryMatrixFunc
{
protected:
	TFunc _Functor;
public:
	typedef TFunc typeFunctor;
	BinaryMatrixFuncT(DblRasterMx & op1,DblRasterMx & op2, DblRasterMx & ret, TFunc & functor):BinaryMatrixFunc(op1,op2,ret),_Functor(functor) {} 
	
	virtual void run();
	virtual paralellisable * clone() const;
	
};


template<class TFunc>
void BinaryMatrixFuncT<TFunc>::run()
{
	if (!areOperandsCompatibile()){
		throw exOperandsNotCompatibile();
	}

	_pRet->initlike(*_pOp1);

	size_t nSize = _pRet->getRowNr()*_pRet->getColNr();

	double * pRetData = _pRet->getData();
	double * pOp1Data = _pOp1->getData();
	double * pOp2Data = _pOp2->getData();

	for ( size_t i = 0; i < nSize; i++, pRetData++, pOp1Data++, pOp2Data++ )
		*pRetData = _Functor(*pOp1Data,*pOp2Data);

}

template<class TFunc>
paralellisable * BinaryMatrixFuncT<TFunc>::clone() const 
{
	return new BinaryMatrixFuncT<TFunc>(*this);
}

}

#endif