#include "InterpolateRasterToSquares.h"


namespace TR 
{

InterpolateRasterToSquares::InterpolateRasterToSquares(DblRasterMx & src, DblRasterMx & target)
: _src(src), _target(target)
{}



void InterpolateRasterToSquares::interpolate()
{
	size_t rowNr = _src.getRowNr();
	size_t colNr = _src.getColNr();
	
	double d = 0.0;
	double pixelSize = _src.getPixelSize();
	eOrigo origo = _src.getOrigo();
	
	if (colNr==0 || rowNr==0){
		_target.init(0,0,pixelSize,origo,d);
		return;
	}

	_target.init(rowNr+1,colNr+1,pixelSize,origo,d);

	DblRasterMx::iterator iSrc = _src.begin();
	DblRasterMx::iterator iEnd = _src.end();
	
	size_t i = 0;
	size_t j = 0;

	for (i = 1; i < rowNr; i++) {
		for ( j = 1; j < colNr; j++) {
			
			double sum = 0.0;
			DblRasterMx::iterator it = _src.getIteratorAt(i-1,j-1);
			
			if (computeValue(it,sum,i,j))
				continue;
			
			it.skipToChainCode(6);

			if (computeValue(it,sum,i,j))
				continue;

			it.skipToChainCode(7);

			if (computeValue(it,sum,i,j))
				continue;

			it.skipToChainCode(6);

			if (computeValue(it,sum,i,j))
				continue;

			_target(i,j) = sum * 0.25;

		}
	}

	for ( i = 1; i < colNr; i++ ){
		
		double sum = 0.0;
		DblRasterMx::iterator it = _src.getIteratorAt(0,i-1);

		if (computeValue(it,sum,0,i))
				continue;

		it.skipToChainCode(6);

		if (computeValue(it,sum,0,i))
				continue;

		_target(0,i) = sum * 0.5;
	}

	for ( i = 1; i < colNr; i++ ){
		
		double sum = 0.0;
		DblRasterMx::iterator it = _src.getIteratorAt(rowNr,i-1);

		if (computeValue(it,sum,rowNr,i))
				continue;

		it.skipToChainCode(6);

		if (computeValue(it,sum,rowNr,i))
				continue;

		_target(rowNr,i) = sum * 0.5;
	}

	for ( i = 1; i < rowNr; i++ ){
		
		double sum = 0.0;
		DblRasterMx::iterator it = _src.getIteratorAt(i-1,0);

		if (computeValue(it,sum,i,0))
				continue;

		it.skipToChainCode(8);

		if (computeValue(it,sum,i,0))
				continue;

		_target(i,0) = sum * 0.5;
	}

	for ( i = 1; i < rowNr; i++ ){
		
		double sum = 0.0;
		DblRasterMx::iterator it = _src.getIteratorAt(i-1,colNr);

		if (computeValue(it,sum,i,colNr))
				continue;

		it.skipToChainCode(8);

		if (computeValue(it,sum,i,colNr))
				continue;

		_target(i,colNr) = sum * 0.5;
	}

	_target(0,0) = _src(0,0);
	_target(0,colNr) = _src(0,colNr-1);
	_target(rowNr,0) = _src(rowNr-1,0);
	_target(rowNr,0) = _src(rowNr-1,colNr-1);

}

DblRasterMx & InterpolateRasterToSquares::getSrc()
{
	return _src;
}

DblRasterMx & InterpolateRasterToSquares::getTarget()
{
	return _target;
}

}
