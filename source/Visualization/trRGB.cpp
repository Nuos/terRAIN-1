#include "trRGB.h"


namespace VISUALIZATION
{

trRGB::trRGB():_r(0.0), _g(0.0), _b(0.0)
{}

trRGB::trRGB( double r, double g, double b )
:_r(r), _g(g), _b(b)
{}

void trRGB::set( double r, double g, double b )
{
	_r = r;
	_g = g;
	_b = b;
}

double trRGB::r() const
{
	return _r;
}

double trRGB::g() const
{
	return _g;
}

double trRGB::b() const
{
	return _b;
}

bool trRGB::isValid() const
{
	return ( (_r>=0.0 && _r<=1.0) &&
		     (_g>=0.0 && _g<=1.0) &&
			 (_b>=0.0 && _b<=1.0));
}

}