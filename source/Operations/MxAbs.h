#ifndef MXABS_H
#define MXABS_H

#include "UnaryMatrixFunc.h"
namespace TR
{

class MxAbs: public UnaryMatrixFunc
{
public:
	MxAbs(DblRasterMx & op, DblRasterMx & ret);
	virtual void run();
	virtual paralellisable * clone() const;
};

}

#endif