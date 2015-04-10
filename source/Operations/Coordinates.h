#ifndef COORDINATES_H
#define COORDINATES_H

#include "rastermatrix.h"
#include "UnaryMatrixFunc.h"
#include "defs.h"

namespace TR
{

class Coordinates: public UnaryMatrixFunc
{
private:
	eCoordType	_CoordType;

public:
	Coordinates(eCoordType CoordType,DblRasterMx & op, DblRasterMx & ret );
	virtual void run();
	virtual paralellisable * clone() const;
};

}

#endif