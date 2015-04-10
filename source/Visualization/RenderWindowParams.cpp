#include "RenderWindowParams.h"

namespace VISUALIZATION
{

RenderWindowParams::RenderWindowParams(int sizeX, int sizeY):
	_sizeX(sizeX),_sizeY(sizeY)
{}

int RenderWindowParams::getSizeX() const
{
	return _sizeX;
}

int RenderWindowParams::getSizeY() const
{
	return _sizeY;
}

}