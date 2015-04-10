#include "MxAbs.h"
#include <cmath>


namespace TR
{

MxAbs::MxAbs(DblRasterMx & op, DblRasterMx & ret):UnaryMatrixFunc(op,ret){}

void MxAbs::run()
{
	if (!_pOp->isValid()){
		return;
	}

	size_t nRows = _pOp->getRowNr();
	size_t nCols = _pOp->getColNr();

	(*_pRet) = *_pOp;

	for (size_t i = 0; i < nRows; i++)
		for (size_t j = 0; j < nCols; j++)
			(*_pRet)(i,j) = std::fabs((*_pOp)(i,j));

}

paralellisable * MxAbs::clone() const
{
	return new MxAbs(*this);
}
}