#include "trPoint3d.h"

trPoint3d::trPoint3d():
_x(0.0),_y(0.0),_z(0.0)
{}

trPoint3d::trPoint3d(double x, double y, double z):
_x(x),_y(y),_z(z)
{}

void trPoint3d::setX(double x)
{
	_x = x;
}

void trPoint3d::setY(double y)
{
	_y = y;
}

void trPoint3d::setZ(double z)
{
	_z = z;
}

void trPoint3d::set(double x, double y, double z)
{
	_x = x;
	_y = y;
	_z = z;
}

double trPoint3d::getX() const
{
	return _x;
}

double trPoint3d::getY() const
{
	return _y;
}

double trPoint3d::getZ() const
{
	return _z;
}