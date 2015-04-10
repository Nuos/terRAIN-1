#ifndef RASTERPOSITIONFUNCTORS_H
#define RASTERPOSITIONFUNCTORS_H

#include "RasterPositionMatrix.h"
#include "defs.h"
namespace FUNC
{

	class funcDiagonalPosition
	{
	public:
		
		void op( TR::RasterPositionMatrix::iterator & iOp, TR::DblRasterMx::iterator & iRet ){
			
			size_t absDeltaRow = (iOp.getRow() == iOp->getRow()) ? 0 : 1; 
			size_t absDeltaCol = (iOp.getCol() == iOp->getCol()) ? 0 : 1; 
			
			if (absDeltaRow==1 && absDeltaCol==1)
				*iRet = SQRT2REC;
			else
				*iRet = 1.0;
		}
	};
}

#endif