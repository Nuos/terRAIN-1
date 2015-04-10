#ifndef BINARYMATRIXFUNC_H
#define BINARYMATRIXFUNC_H

#include "MatrixFunc.h"

namespace TR
{

class BinaryMatrixFunc: public MatrixFunc
{
protected:
	DblRasterMx * _pOp1;
	DblRasterMx * _pOp2;

public:
	BinaryMatrixFunc(DblRasterMx & op1,DblRasterMx & op2, DblRasterMx & ret): MatrixFunc(ret),_pOp1(&op1),_pOp2(&op2){} 

	bool areOperandsCompatibile() const;

	DblRasterMx & getOp1()
	{
		return *_pOp1;
	}

	DblRasterMx & getOp2()
	{
		return *_pOp2;
	}

	void setOp1( DblRasterMx & op1 )
	{
		_pOp1 = &op1;
	}

	void setOp2( DblRasterMx & op2 )
	{
		_pOp2 = &op2;
	}
};

}

#endif BINARYMATRIXFUNC_H