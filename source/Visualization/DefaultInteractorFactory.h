#ifndef DEFAULTINTERACTORFACTORY_H
#define DEFAULTINTERACTORFACTORY_H

#include "InteractorFactoryBase.h"

namespace VISUALIZATION
{

class DefaultInteractorFactory : public InteractorFactoryBase
{
public:
	virtual vtkRenderWindowInteractor * create(){
		return vtkRenderWindowInteractor::New();
	}
};

}
#endif