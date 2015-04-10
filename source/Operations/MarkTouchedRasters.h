#ifndef MARKEDTOUCHEDRASTERS_H
#define MARKEDTOUCHEDRASTERS_H

#include "MLDDWalkFunction.h"
#include "rastermatrix.h"
#include "MarkCondition.h"


namespace TR {

class MarkTouchedRasters : public MLDDWalkFunction
{
private:
	DblRasterMx & _markMx;
	double _mark;
	size_t _cnt;
	MarkCondition * condition;
public:
	MarkTouchedRasters(DblRasterMx & markMx, double mark);
	virtual ~MarkTouchedRasters();
	size_t getCnt() const;
	virtual bool operator()(const MultiflowDMatrix::iterator & it);
	void setMark(double mark);
	void inc(double d);
	void setCondition(MarkCondition * cond);
};

}

#endif