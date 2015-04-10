#ifndef SIMPLERENDERWINDOWFACTORY_H
#define SIMPLERENDERWINDOWFACTORY_H

#include "RenderWindowFactoryBase.h"
#include "RenderWindowParams.h"

namespace VISUALIZATION
{

	class SimpleRenderWindowFactory : public RenderWindowFactoryBase
{
	RenderWindowParams _params;
public:
	SimpleRenderWindowFactory( const RenderWindowParams & params);
	virtual vtkRenderWindow * create();
};

}

#endif