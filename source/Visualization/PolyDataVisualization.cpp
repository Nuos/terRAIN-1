#include "PolyDataVisualization.h"


namespace VISUALIZATION
{

void PolyDataVisualization::init()
{
	_polyDataActor->SetMapper(_polyDataMapper);
	_renderer->AddActor(_polyDataActor);
	_polyDataMapper->SetInput(_data);
}

PolyDataVisualization::PolyDataVisualization(RendererFactoryBase & rendererFactory, RenderWindowFactoryBase & windowFactory, InteractorFactoryBase & interactorFactory, PolyDataFactory & polyDataFactory):
	VisualizationBase(rendererFactory,windowFactory,interactorFactory)
{
	_data = polyDataFactory.create();

	_polyDataActor = vtkActor::New();
	_polyDataMapper = vtkPolyDataMapper::New();
}

PolyDataVisualization::~PolyDataVisualization()
{
	_data->Delete();
	_polyDataActor->Delete();
	_polyDataMapper->Delete();
}

}