#include "TerrainRenderWindow.h"


#include "vtkPoints.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"
#include "vtkCamera.h"

TerrainRenderWindow::TerrainRenderWindow( RenderWindowParams & params )
	: RenderWindowBase(params),_terrain(NULL)
{
	_actor = vtkActor::New();
	_polyDataMapper = vtkPolyDataMapper::New();
	_actor->SetMapper(_polyDataMapper);

	// The usual rendering stuff.
	_camera = vtkCamera::New();
	_camera->SetPosition(1,1,1);
	_camera->SetFocalPoint(0,0,0);
	
	_renderer->SetActiveCamera(_camera);
	_renderer->AddActor(_actor);
}


void TerrainRenderWindow::initRenderWindow()
{
	_renderer->ResetCamera();
    _renderer->SetBackground(1,1,1);    
}

void TerrainRenderWindow::setTerrain(const DblRasterMx & mx)
{
	if (_terrain!=NULL)
		_terrain->Delete();

	_terrain = vtkPolyData::New();
	vtkPoints * points = vtkPoints::New();
	vtkCellArray *polys = vtkCellArray::New();
	vtkFloatArray *scalars = vtkFloatArray::New();

	size_t rows = mx.getRowNr();
	size_t cols = mx.getColNr();

	double pixelSize = mx.getPixelSize();
	double x = 0;
	double y = 0;

	
	size_t i = 0;
	size_t j = 0;

	for (i = 0; i < rows; i++){
		x = 0;
		for (j = 0; j < cols; j++){
			points->InsertPoint(static_cast<vtkIdType>(mx.getIndexOfPos(i,j)),x,y,mx(i,j));
			x+=pixelSize;
		}
		y+=pixelSize;
	}
	
	static vtkIdType pts[4]={0,1,2,3};

	for (i = 0; i < rows-1; i++){
		
		for (j = 0; j < cols-1; j++){
			pts[0] = static_cast<vtkIdType>(mx.getIndexOfPos(i,j));
			pts[1] = static_cast<vtkIdType>(mx.getIndexOfPos(i+1,j));
			pts[2] = static_cast<vtkIdType>(mx.getIndexOfPos(i+1,j+1));
			pts[3] = static_cast<vtkIdType>(mx.getIndexOfPos(i,j+1));

			polys->InsertNextCell(4,pts);
		}

		//scalars->InsertTuple1(i,i);
		
	}


	_terrain->SetPoints(points);
	points->Delete();
	_terrain->SetPolys(polys);
	polys->Delete();
	//_terrain->GetPointData()->SetScalars(scalars);
	scalars->Delete();

	_polyDataMapper->SetInput(_terrain);

}

TerrainRenderWindow::~TerrainRenderWindow()
{
	if (_terrain)
		_terrain->Delete();

	_polyDataMapper->Delete();
	_actor->Delete();
	_camera->Delete();
}