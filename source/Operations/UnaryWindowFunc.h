#ifndef UNARYWINDOWFUNC_H
#define UNARYWINDOWFUNC_H

#include "rastermatrix.h"

namespace TR
{

class UnaryWindowFunc : public paralellisable
{
protected:
	DblRasterMx::iterator _RetIterator;
	DblRasterMx::iterator _OpBegin;
	DblRasterMx::iterator _OpEnd;

public:
	UnaryWindowFunc( DblRasterMx::iterator & iOpBegin, DblRasterMx::iterator & iOpEnd, DblRasterMx::iterator & iRet) 
		: _RetIterator(iRet), _OpBegin(iOpBegin), _OpEnd(iOpEnd)
	{}

	DblRasterMx::iterator & getRetIterator()
	{
		return _RetIterator;
	}

	DblRasterMx::iterator & getOpBegin()
	{
		return _OpBegin;
	}

	DblRasterMx::iterator & getOpEnd()
	{
		return _OpEnd;
	}

	void setOpBegin( DblRasterMx::iterator & src )
	{
		_OpBegin = src;
	}

	void setOpEnd( DblRasterMx::iterator & src )
	{
		_OpEnd = src;
	}

	void setRetIterator( DblRasterMx::iterator & src )
	{
		_RetIterator = src;
	}
};

}

#endif