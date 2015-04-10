#include "RenderWindowBase.h"

RenderWindowBase::RenderWindowBase(RenderWindowParams & params):_params(params)
{
	_renWin = vtkRenderWindow::New();
	_renWin->SetSize(params.getSizeX(),params.getSizeY());
	_renderer = vtkRenderer::New();
	_interactor = vtkRenderWindowInteractor::New();

	_renWin->AddRenderer(_renderer);
	_interactor->SetRenderWindow(_renWin);

}

void RenderWindowBase::display()
{	

	initRenderWindow();

	_renWin->Render();
	_interactor->Start();
}

RenderWindowBase::~RenderWindowBase()
{
	_renWin->Delete();
	_renderer->Delete();
	_interactor->Delete();
}