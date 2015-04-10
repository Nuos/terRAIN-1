#include "VisualizationParams.h"

namespace VISUALIZATION
{

VisualizationParams::VisualizationParams(): 
	_renderWindowSizeX(300),_renderWindowSizeY(400)
{}
	
VisualizationParams::VisualizationParams(int renderWindowSizeX, int renderWindowSizeY):
	_renderWindowSizeX(renderWindowSizeX),_renderWindowSizeY(renderWindowSizeY)
{}

int VisualizationParams::getRenderWindowSizeX() const
{
	return _renderWindowSizeX;
}

int VisualizationParams::getRenderWindowSizeY() const
{
	return _renderWindowSizeY;
}

void VisualizationParams::setRenderWindowSizeX( int renderWindowSizeX )
{
	_renderWindowSizeX = renderWindowSizeX;
}

void VisualizationParams::setRenderWindowSizeY( int renderWindowSizeY )
{
	_renderWindowSizeY = renderWindowSizeY;
}
}