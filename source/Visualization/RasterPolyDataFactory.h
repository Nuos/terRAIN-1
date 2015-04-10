#ifndef RASTERPOLYDATAFACTORY_H
#define RASTERPOLYDATAFACTORY_H

#include "PolyDataFactory.h"
#include "rastermatrix.h"

using namespace TR;

namespace VISUALIZATION
{

class RasterPolyDataFactory: public PolyDataFactory
{
	const DblRasterMx & _mx;
public:
	RasterPolyDataFactory(const DblRasterMx & mx);
	virtual vtkPolyData * create();

};

}

#endif