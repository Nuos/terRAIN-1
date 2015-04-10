#include "BinaryMatrixFunc.h"

namespace TR
{
	bool BinaryMatrixFunc::areOperandsCompatibile() const
	{
		if (!(_pOp1->isValid() && _pOp2->isValid()))
			return false;
		
		if (_pOp1->getRowNr()!=_pOp2->getRowNr() || _pOp1->getColNr()!=_pOp2->getColNr())
			return false;

		return true;
	}
}