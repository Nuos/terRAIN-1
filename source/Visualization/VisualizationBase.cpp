#include "VisualizationBase.h"

namespace VISUALIZATION
{

VisualizationBase::VisualizationBase(RendererFactoryBase & rendererFactory, RenderWindowFactoryBase & windowFactory, InteractorFactoryBase & interactorFactory)
{
	_inited = false;

	_renderer = rendererFactory.create();
	_renderWindow = windowFactory.create();
	_interactor = interactorFactory.create();
	_renderWindow->AddRenderer(_renderer);
	_interactor->SetRenderWindow(_renderWindow);
}

VisualizationBase::~VisualizationBase()
{
	_interactor->Delete();
	_renderWindow->Delete();
	_renderer->Delete();
}

void VisualizationBase::visualize()
{	
	// call init only one time
	if (!_inited){
		init();
		_inited = true;
	}

	_renderWindow->Render();
	_interactor->Start();
}

}