#ifndef POLYDATAFACTORY_H
#define POLYDATAFACTORY_H

#include "vtkPolyData.h"

namespace VISUALIZATION
{

class PolyDataFactory
{
public:
	virtual vtkPolyData * create() = 0;
};

}

#endif