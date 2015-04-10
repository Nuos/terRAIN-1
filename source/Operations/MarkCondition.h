#ifndef MARKCONDITION_H
#define MARKCONDITION_H
#include "rastermatrix.h"

namespace TR {

class MarkCondition
{
public:
	virtual bool check(size_t row, size_t col, DblRasterMx & mx) = 0;
};

}

#endif