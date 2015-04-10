#ifndef INTERPOLATERASTERTOSQUARES_H
#define INTERPOLATERASTERTOSQUARES_H

#include "rastermatrix.h"
#include "utils.h"

namespace TR {

class InterpolateRasterToSquares
{
protected:
	DblRasterMx & _src;
	DblRasterMx & _target;
public:
	InterpolateRasterToSquares(DblRasterMx & src, DblRasterMx & target);

	void interpolate();

	DblRasterMx & getSrc();
	DblRasterMx & getTarget();

private:
	inline bool computeValue( DblRasterMx::iterator & it, double & val, size_t targetRow, size_t targetCol);
};

inline bool InterpolateRasterToSquares::computeValue( DblRasterMx::iterator & it, double & val, size_t targetRow, size_t targetCol)
{
	if (/*isPIT(it) || isRidgePoint(it)*/false){
		_target(targetRow,targetCol)=*it;
		return true;
	}else
		val+=*it;

	return false;
}

}

#endif