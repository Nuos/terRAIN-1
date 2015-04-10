#ifndef COLLECTTOUCHEDRASTERS_H
#define COLLECTTOUCHEDRASTERS_H

#include "MLDDWalkFunction.h"
#include "rastermatrix.h"
#include "MarkCondition.h"
#include <vector>

namespace TR {

class CollectTouchedRasters : public MLDDWalkFunction
{
private:
	DblRasterMx & _markMx;
	MarkCondition * _condition;
	std::vector<MultiflowDMatrix::iterator> & _collection;
public:
	CollectTouchedRasters(DblRasterMx & markMx, std::vector<MultiflowDMatrix::iterator> & collection);
	virtual ~CollectTouchedRasters();
	virtual bool operator()(const MultiflowDMatrix::iterator & it);
	void setCondition(MarkCondition * cond);
};

}

#endif