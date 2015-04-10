#ifndef ACCFLUXBASE_H
#define ACCFLUXBASE_H

#include "paralellisable.h"

namespace TR
{

class accfluxBase : public paralellisable
{
protected:
	DblRasterMx	   * _pDepthMx;
	DblRasterMx	   * _pRetMx;
public:
	accfluxBase(DblRasterMx & mxDepth,DblRasterMx & mxRet):_pDepthMx(&mxDepth), _pRetMx(&mxRet) 
	{}

	DblRasterMx & getDepthMx()
	{
		return * _pDepthMx;
	}

	DblRasterMx & getRetMx()
	{
		return * _pRetMx;
	}

	void setDepthMx( DblRasterMx & mxDepth )
	{
		_pDepthMx = & mxDepth;	
	}

	void setRetMx( DblRasterMx & mxRet )
	{
		_pRetMx = & mxRet;
	}
};

}

#endif