#ifndef MLDDWALKFUNCTION_H
#define MLDDWALKFUNCTION_H

#include "MultiflowDMatrix.h"

namespace TR
{

class MLDDWalkFunction
{
public:
	virtual bool operator()(const MultiflowDMatrix::iterator & it)=0;
	virtual ~ MLDDWalkFunction(){}
};


class DummyWalkFunction : public MLDDWalkFunction
{
public:
	virtual bool operator()(const MultiflowDMatrix::iterator & it)
	{
		return true;
	}
};

}

#endif