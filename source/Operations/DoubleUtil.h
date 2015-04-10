#ifndef DOUBLEUTIL_H
#define DOUBLEUTIL_H

#include <cmath>
#include <cfloat>
#include <limits>

namespace TR
{

class DoubleUtil
{
public:
	static const double sDeltaD6;
	static const double sDeltaD3;

	static inline double getNAN()
	{
		return std::numeric_limits<double>::quiet_NaN();
	}

	static bool inline isNAN(double dblVal)
	{
#ifdef isnan
		return isnan(dblVal)!=0;
#else
		return _isnan(dblVal)!=0;
#endif
	}

	static bool inline cmpD6(double dblVal1, double dblVal2)
	{
		return std::fabs(dblVal1 - dblVal2)<sDeltaD6;
	}

	static bool inline cmpD3(double dblVal1, double dblVal2)
	{
		return (std::fabs(dblVal1 - dblVal2) < sDeltaD3);
	}

	static double inline getMAXValue()
	{
		return std::numeric_limits<double>::max();
	}

	static double inline getMINValue()
	{
		return std::numeric_limits<double>::min();
	}

	static double inline infinite()
	{
		return std::numeric_limits<double>::infinity();
	}
	
	static bool inline isInfinite(double dblVal)
	{
#ifdef finite
		return finite(dblVal)==0;
#else
		return _finite(dblVal)==0;
#endif
	}
};

}

#endif DOUBLEUTIL_H