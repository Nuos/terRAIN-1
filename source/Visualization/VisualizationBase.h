#ifndef VISUALIZATIONBASE_H
#define VISUALIZATIONBASE_H

#include "RendererFactoryBase.h"
#include "RenderWindowFactoryBase.h"
#include "InteractorFactoryBase.h"
#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "vtkRenderWindowInteractor.h"

namespace VISUALIZATION
{

class VisualizationBase
{
private:
	bool _inited;
protected:
	vtkRenderer * _renderer;
	vtkRenderWindow * _renderWindow;
	vtkRenderWindowInteractor * _interactor;
public:
	virtual void visualize();
	virtual void init()=0;
	
	VisualizationBase(RendererFactoryBase & rendererFactory, RenderWindowFactoryBase & windowFactory, InteractorFactoryBase & interactorFactory);
	virtual ~VisualizationBase();


};

}

#endif