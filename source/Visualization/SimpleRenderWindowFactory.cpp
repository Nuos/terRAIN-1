#include "SimpleRenderWindowFactory.h"

namespace VISUALIZATION
{

SimpleRenderWindowFactory::SimpleRenderWindowFactory( const RenderWindowParams & params):
_params(params)
{}

vtkRenderWindow * SimpleRenderWindowFactory::create()
{
	vtkRenderWindow * renderWindow = vtkRenderWindow::New();
	renderWindow->SetSize(_params.getSizeX(),_params.getSizeY());

	return renderWindow;
}

}