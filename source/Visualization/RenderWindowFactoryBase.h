#ifndef RENDERWINDOWFACTORYBASE_H
#define RENDERWINDOWFACTORYBASE_H

#include "vtkRenderWindow.h"

namespace VISUALIZATION
{

class RenderWindowFactoryBase
{
public:
	virtual vtkRenderWindow * create() = 0;
};

}

#endif