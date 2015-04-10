#ifndef UNARYMATRIXFUNC_H
#define UNARYMATRIXFUNC_H

#include "MatrixFunc.h"

namespace TR
{

class UnaryMatrixFunc: public MatrixFunc
{
protected:
	DblRasterMx * _pOp;

public:
	UnaryMatrixFunc(DblRasterMx & op, DblRasterMx & ret): MatrixFunc(ret),_pOp(&op){} 

	DblRasterMx & getOp()
	{
		return *_pOp;
	}

	void setOp( DblRasterMx & op )
	{
		_pOp = &op;
	}
};

}

#endif UNARYMATRIXFUNC_H