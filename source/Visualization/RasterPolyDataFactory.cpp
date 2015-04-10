#include "RasterPolyDataFactory.h"
#include "vtkCellArray.h"
#include "vtkFloatArray.h"
#include "vtkPointData.h"

namespace VISUALIZATION
{

RasterPolyDataFactory::RasterPolyDataFactory(const DblRasterMx & mx):_mx(mx)
{}

vtkPolyData * RasterPolyDataFactory::create()
{
	vtkPolyData * terrain = vtkPolyData::New();
	vtkPoints * points = vtkPoints::New();
	vtkCellArray *polys = vtkCellArray::New();
	vtkFloatArray *scalars = vtkFloatArray::New();

	size_t rows = _mx.getRowNr();
	size_t cols = _mx.getColNr();

	double pixelSize = _mx.getPixelSize();
	double x = 0;
	double y = 0;

	
	size_t i = 0;
	size_t j = 0;

	for (i = 0; i < rows; i++){
		x = 0;
		for (j = 0; j < cols; j++){
			points->InsertPoint(static_cast<vtkIdType>(_mx.getIndexOfPos(i,j)),x,y,_mx(i,j));
			x+=pixelSize;
		}
		y+=pixelSize;
	}
	
	static vtkIdType pts[4]={0,1,2,3};

	for (i = 0; i < rows-1; i++){
		
		for (j = 0; j < cols-1; j++){
			pts[0] = static_cast<vtkIdType>(_mx.getIndexOfPos(i,j));
			pts[1] = static_cast<vtkIdType>(_mx.getIndexOfPos(i+1,j));
			pts[2] = static_cast<vtkIdType>(_mx.getIndexOfPos(i+1,j+1));
			pts[3] = static_cast<vtkIdType>(_mx.getIndexOfPos(i,j+1));

			polys->InsertNextCell(4,pts);
		}
	}


	terrain->SetPoints(points);
	points->Delete();
	terrain->SetPolys(polys);
	polys->Delete();
	
	scalars->Delete();

	return terrain;
}

}