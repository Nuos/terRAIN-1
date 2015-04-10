#ifndef RENDERERFACTORYPARAMS_H
#define RENDERERFACTORYPARAMS_H

#include "trPoint3d.h"
#include "trRGB.h"

namespace VISUALIZATION
{

class RendererFactoryParams
{
protected:
	trPoint3d _cameraPos;
	trPoint3d _cameraFocusPoint;
	trRGB	  _ambientColor;
	trRGB	  _backgroundColor;
public:
	RendererFactoryParams();
	RendererFactoryParams(const trPoint3d & cameraPos, const trPoint3d & cameraFocusPoint, const trRGB & ambientColor,const trRGB & backgroundColor);

	void setCameraPos(const trPoint3d & cameraPos);
	void setCameraFocusPoint(const trPoint3d & cameraFocusPoint);
	void setAmbientColor( const trRGB & ambientColor );
	void setBackgroundColor( const trRGB & backgroundColor );
	
	trPoint3d getCameraPos() const;
	trPoint3d getCameraFocusPoint() const;
	trRGB getAmbientColor() const;
	trRGB getBackgroundColor() const;


};

}
#endif