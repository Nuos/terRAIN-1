#ifndef MATRIXFUNC_H
#define MATRIXFUNC_H

#include "paralellisable.h"
#include "rastermatrix.h"

namespace TR
{

class MatrixFunc: public paralellisable
{
protected:
	DblRasterMx * _pRet;
public:
	MatrixFunc(	DblRasterMx & ret):_pRet(&ret){}
	
	DblRasterMx & getRet()
	{
		return *_pRet;
	}

	void setRet( DblRasterMx & ret )
	{
		_pRet = &ret;	
	}
};

}



#endif