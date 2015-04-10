#ifndef INTERACTORFACTORYBASE_H
#define INTERACTORFACTORYBASE_H

#include "vtkRenderWindowInteractor.h"

namespace VISUALIZATION
{

class InteractorFactoryBase
{
public:
	virtual vtkRenderWindowInteractor * create() = 0;
};

}

#endif