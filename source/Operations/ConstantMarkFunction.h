#ifndef CONSTANTMARKFUNCTION_H
#define CONSTANTMARKFUNCTION_H

#include "MarkFunction.h"

namespace TR
{

class ConstantMarkFunction : public MarkFunction
{
private:
	double val;
public:
	ConstantMarkFunction(double v):val(v){}

	virtual double mark(const MultiflowDMatrix::iterator & it){
		return val;
	}
};

}

#endif