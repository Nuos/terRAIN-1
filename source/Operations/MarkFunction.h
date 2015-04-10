#ifndef MARKFUNCTION_H
#define MARKFUNCTION_H

#include "MultiflowDMatrix.h"

namespace TR
{

class MarkFunction
{
public:
	virtual double mark(const MultiflowDMatrix::iterator & it)=0;
};

}
#endif