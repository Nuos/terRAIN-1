#ifndef FILLMLDDPITS_H
#define FILLMLDDPITS_H

#include "paralellisable.h"
#include "MultiflowDMatrix.h"
#include <list>
#include "rastermatrix.h"
#include <iostream>

namespace TR
{

class FillMLDDPits : public paralellisable
{

private:
	MultiflowDMatrix & _mxMLDD;
	MultiflowDMatrix & _mxRet;
	

public:
	FillMLDDPits(MultiflowDMatrix & mxMLDD, MultiflowDMatrix & mxRet);
	DblRasterMx mx;
	virtual void run();
	virtual paralellisable * clone() const;
	
	

//private:
	void markAvailableRasters(MultiflowDMatrix::iterator iter, DblRasterMx & mx, double sign) const;
	void invertNotMarkedRasters(MultiflowDMatrix & mxSource,MultiflowDMatrix & mxTarget, DblRasterMx & mx);

};

}

#endif