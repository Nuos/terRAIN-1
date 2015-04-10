#ifndef MXADD_H
#define MXADD_H

#include "BinaryMatrixFunc.h"

namespace TR
{

class MxAdd : public BinaryMatrixFunc
{
public:
	MxAdd( DblRasterMx & op1, DblRasterMx & op2, DblRasterMx & ret);
	virtual void run();
	virtual paralellisable * clone() const;
};

}

#endif