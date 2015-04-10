#ifndef TERRAINRENDERWINDOW_H
#define TERRAINRENDERWINDOW_H

#include "RenderWindowBase.h"
#include "rastermatrix.h"
#include "vtkActor.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyData.h"

using namespace TR;


class TerrainRenderWindow : public RenderWindowBase 
{
protected: 
	vtkActor * _actor;
	vtkPolyDataMapper * _polyDataMapper;
	vtkPolyData * _terrain;
	vtkCamera * _camera;

public:
	TerrainRenderWindow( RenderWindowParams & params );
	virtual void initRenderWindow();
	void setTerrain(const DblRasterMx & mx);
	~TerrainRenderWindow();
};

#endif