#include "RendererFactoryParams.h"

namespace VISUALIZATION
{

RendererFactoryParams::RendererFactoryParams():
_cameraPos(-50,-50,130),_cameraFocusPoint(50,50.0,100.0),_ambientColor(1.0,1.0,1.0),_backgroundColor(1.0,1.0,1.0)
{}

RendererFactoryParams::RendererFactoryParams(const trPoint3d & cameraPos, const trPoint3d & cameraFocusPoint, const trRGB & ambientColor,const trRGB & backgroundColor)
:_cameraPos(cameraPos),_cameraFocusPoint(cameraFocusPoint),_ambientColor(ambientColor),_backgroundColor(backgroundColor)
{}

void RendererFactoryParams::setCameraPos(const trPoint3d & cameraPos)
{
	_cameraPos = cameraPos;
}

void RendererFactoryParams::setCameraFocusPoint(const trPoint3d & cameraFocusPoint)
{
	_cameraFocusPoint = cameraFocusPoint;
}

void RendererFactoryParams::setAmbientColor( const trRGB & ambientColor )
{
	_ambientColor = ambientColor;
}

trPoint3d RendererFactoryParams::getCameraPos() const
{
	return _cameraPos;
}

trPoint3d RendererFactoryParams::getCameraFocusPoint() const
{
	return _cameraFocusPoint;
}

trRGB RendererFactoryParams::getAmbientColor() const
{
	return _ambientColor;
}

void RendererFactoryParams::setBackgroundColor( const trRGB & backgroundColor )
{
	_backgroundColor = 	backgroundColor;
}

trRGB RendererFactoryParams::getBackgroundColor() const
{
	return _backgroundColor;
}

}