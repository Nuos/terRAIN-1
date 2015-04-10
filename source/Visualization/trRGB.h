#ifndef TRRGB_H
#define TRRGB_H

namespace VISUALIZATION
{


class trRGB
{
private:

	double _r;
	double _g;
	double _b;

public:
	
	trRGB();
	trRGB( double r, double g, double b );

	void set( double r, double g, double b );

	double r() const;
	double g() const;
	double b() const;

	bool isValid() const;
};

}
#endif