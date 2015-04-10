#include "Coordinates.h"

namespace TR
{

Coordinates::Coordinates(eCoordType CoordType,DblRasterMx & op, DblRasterMx & ret ): UnaryMatrixFunc(op, ret), _CoordType(CoordType)
{
}

void Coordinates::run()
{
	
	
	DblRasterMx & mxRet = getRet();
	DblRasterMx & mxOp	= getOp();
	
	if (!mxOp.isValid())
		return;

	mxRet.initlike(mxOp);

	size_t nRows = mxOp.getRowNr();
	size_t nCols = mxOp.getColNr();
	size_t i	 = 0;
	size_t j	 = 0;

	double dblPixelSize = mxOp.getPixelSize();
	double dblCoord		= 0.5 * mxOp.getPixelSize();

	if (_CoordType==ctY){
		
		for ( i = 0; i < nRows; i++ ){
			
			for ( j = 0; j < nCols; j++) {
				mxRet(i,j) = dblCoord;	
			}

			dblCoord+=dblPixelSize;
		}
		

	}else {
		

		for ( i = 0; i < nCols; i++ ){
			
			for ( j = 0; j < nRows; j++) {
				mxRet(j,i) = dblCoord;	
			}

			dblCoord+=dblPixelSize;
		}
		
	}

}

paralellisable * Coordinates::clone() const
{
	return new Coordinates(_CoordType,*_pOp,*_pRet);
}


}