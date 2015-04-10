#ifndef RENDERERFACTORY_H
#define RENDERERFACTORY_H

#include "RendererFactoryBase.h"
#include "RendererFactoryParams.h"

namespace VISUALIZATION
{

class RendererFactory : public RendererFactoryBase
{

protected:
	const RendererFactoryParams & _params;
public:
	RendererFactory(const RendererFactoryParams & params);
	virtual vtkRenderer * create();
};

}

#endif