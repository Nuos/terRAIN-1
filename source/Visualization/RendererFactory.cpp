#include "RendererFactory.h"

#include "vtkCamera.h"

namespace VISUALIZATION
{

RendererFactory::RendererFactory(const RendererFactoryParams & params):_params(params)
{}

vtkRenderer * RendererFactory::create()
{
	vtkRenderer * renderer = vtkRenderer::New();
	
	vtkCamera * camera = renderer->GetActiveCamera();
	
	trPoint3d cameraPos = _params.getCameraPos();
	camera->SetPosition(cameraPos.getX(),cameraPos.getY(),cameraPos.getZ());
	
	trPoint3d focalPoint = _params.getCameraFocusPoint();
	camera->SetFocalPoint(focalPoint.getX(),focalPoint.getY(),focalPoint.getZ());

	trRGB ambientColor = _params.getAmbientColor();
	
	renderer->SetAmbient(ambientColor.r(),ambientColor.g(),ambientColor.b());
	
	trRGB backgroundColor = _params.getBackgroundColor();
	
	renderer->SetBackground(backgroundColor.r(),backgroundColor.g(),backgroundColor.b());

	return renderer;
	
}

}