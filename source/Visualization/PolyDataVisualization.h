#ifndef POLYDATAVISUALIZATION_H
#define POLYDATAVISUALIZATION_H

#include "VisualizationBase.h"
#include "PolyDataFactory.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"

namespace VISUALIZATION
{

class PolyDataVisualization : public VisualizationBase
{
protected:
	vtkPolyData * _data;
	vtkPolyDataMapper * _polyDataMapper;
	vtkActor * _polyDataActor;
public:
	virtual void init();
	PolyDataVisualization(RendererFactoryBase & rendererFactory, RenderWindowFactoryBase & windowFactory, InteractorFactoryBase & interactorFactory, PolyDataFactory & polyDataFactory);
	~PolyDataVisualization();
};

}
#endif