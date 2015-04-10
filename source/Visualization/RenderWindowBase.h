#ifndef RENDERWINDOWBASE_H
#define RENDERWINDOWBASE_H

#include "vtkRenderWindow.h"
#include "vtkRenderer.h"
#include "RenderWindowParams.h"
#include "vtkRenderWindowInteractor.h"

using namespace VISUALIZATION;

class RenderWindowBase
{
protected:
	vtkRenderWindow * _renWin;
	vtkRenderer * _renderer;
	vtkRenderWindowInteractor * _interactor;

	RenderWindowParams & _params;
public:
	RenderWindowBase(RenderWindowParams & params);
	
	virtual void initRenderWindow()=0;
	void display();
	virtual ~RenderWindowBase();

};

#endif