#ifndef BINARYWINDOWFUNC_H
#define BINARYWINDOWFUNC_H

#include "rastermatrix.h"

namespace TR
{

class BinaryWindowFunc : public paralellisable
{
protected:
	DblRasterMx::iterator _RetIterator;
	DblRasterMx::iterator _Op1Begin;
	DblRasterMx::iterator _Op1End;
	DblRasterMx::iterator _Op2Begin;

public:
	BinaryWindowFunc( DblRasterMx::iterator & iOp1Begin, DblRasterMx::iterator & iOp1End, DblRasterMx::iterator & iOp2Begin, DblRasterMx::iterator & iRet) 
		: _RetIterator(iRet), _Op1Begin(iOp1Begin), _Op1End(iOp1End), _Op2Begin(iOp2Begin)
	{}

	DblRasterMx::iterator & getRetIterator()
	{
		return _RetIterator;
	}

	DblRasterMx::iterator & getOp1Begin()
	{
		return _Op1Begin;
	}

	DblRasterMx::iterator & getOp1End()
	{
		return _Op1End;
	}

	DblRasterMx::iterator & getOp2Begin()
	{
		return _Op2Begin;
	}

	void setOp1Begin( DblRasterMx::iterator & src )
	{
		_Op1Begin = src;
	}

	void setOp1End( DblRasterMx::iterator & src )
	{
		_Op1End = src;
	}

	void setOp2Begin( DblRasterMx::iterator & src )
	{
		_Op2Begin = src;
	}

	void setRetIterator( DblRasterMx::iterator & src )
	{
		_RetIterator = src;
	}
};

}


#endif