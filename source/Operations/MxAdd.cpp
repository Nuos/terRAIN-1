#include "MxAdd.h"
#include "exOperandsNotCompatibile.h"

namespace TR
{

MxAdd::MxAdd( DblRasterMx & op1, DblRasterMx & op2, DblRasterMx & ret):BinaryMatrixFunc(op1,op2,ret){}

void MxAdd::run()
{
	if (!areOperandsCompatibile()){
		throw exOperandsNotCompatibile();
		return;
	}

	size_t nRowNr = _pOp1->getRowNr();
	size_t nColNr = _pOp1->getColNr();

	(*_pRet) = *_pOp1;

	for (size_t i = 0; i < nRowNr; i++)
		for (size_t j = 0; j < nRowNr; j++)
			(*_pRet)(i,j)+=(*_pOp2)(i,j);
}

paralellisable * MxAdd::clone() const
{
	return new MxAdd(*this);
}

}