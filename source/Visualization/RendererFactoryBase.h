#ifndef RENDERERFACTORYBASE_H
#define RENDERERFACTORYBASE_H

#include "vtkRenderer.h"

namespace VISUALIZATION
{

class RendererFactoryBase
{
public:
	virtual vtkRenderer * create() = 0;
};

}
#endif