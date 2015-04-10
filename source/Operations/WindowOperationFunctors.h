#ifndef WINDOWSOPERATIONFUNCTORS_H
#define WINDOWSOPERATIONFUNCTORS_H

#include "utils.h"
#include "DoubleUtil.h"
#include <cstdlib>
#include "WindowIterator.h"
#include "rastermatrix.h"
#include "ModelParams.h"

namespace FUNC
{

class funcWindowOpBase
{
protected:
	int    _radius;
public:
	funcWindowOpBase(double length, double pixelSize){
		
		double d = pixelSize > DoubleUtil::sDeltaD6 ? length/pixelSize : 0;
		
		_radius = static_cast<int>(d);

		if ( d-_radius>0.5)
			_radius++;
	  }


	int getWindowRadius() const {
		return _radius;	
	}
};

class funcWindowAvarage: public funcWindowOpBase
{

public:
	funcWindowAvarage(double length, double pixelSize): funcWindowOpBase(length,pixelSize){}
	
	inline void op(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		safeop(iOp,iRet );
	}

	inline void safeop(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{	
		
		double sum = 0.0;
		size_t cnt = 0;
		double tmp = 0;
		
		int i = 0;
		int j = 0;
		
		for (i=-_radius; i<=_radius;i++){
			for (j = -_radius; j<=_radius; j++){
				if (iOp.getItemSafe(i,j,tmp)){
					sum+=tmp;
					++cnt;
				}
			}
		}

		if (cnt>0){
			*iRet=sum/cnt;
		}else
			*iRet=0.0; //theorically, this is impossible
	}
};

class funcWindowMax: public funcWindowOpBase
{

public:
	funcWindowMax(double length, double pixelSize): funcWindowOpBase(length,pixelSize){}
	
	inline void op(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		safeop(iOp,iRet );
	}

	inline void safeop(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{	
		
		double max = *iOp;
		double tmp = 0;
		
		int i = 0;
		int j = 0;
		
		for (i=-_radius; i<=_radius;i++){
			for (j = -_radius; j<=_radius; j++){
				if (iOp.getItemSafe(i,j,tmp)){			
					if (tmp>max)
						max = tmp;
				}
			}
		}
		*iRet = max;
	}
};

class funcWindowMin: public funcWindowOpBase
{

public:
	funcWindowMin(double length, double pixelSize): funcWindowOpBase(length,pixelSize){}
	
	inline void op(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		safeop(iOp,iRet );
	}

	inline void safeop(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{	
		
		double min = *iOp;
		double tmp = 0;
		
		int i = 0;
		int j = 0;
		
		for (i=-_radius; i<=_radius;i++){
			for (j = -_radius; j<=_radius; j++){
				if (iOp.getItemSafe(i,j,tmp)){			
					if (tmp<min)
						min = tmp;
				}
			}
		}
		*iRet = min;
	}
};

class funcWindowTotal: public funcWindowOpBase
{

public:
	funcWindowTotal(double length, double pixelSize): funcWindowOpBase(length,pixelSize){}
	
	inline void op(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		safeop(iOp,iRet );
	}

	inline void safeop(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{	
		
		double sum = 0.0;
		double tmp = 0;
		
		int i = 0;
		int j = 0;
		
		for (i=-_radius; i<=_radius;i++){
			for (j = -_radius; j<=_radius; j++){
				if (iOp.getItemSafe(i,j,tmp)){
					sum+=tmp;
				}
			}
		}
		
		*iRet=sum;
	}
};

class funcWindowVariance: public funcWindowOpBase
{

public:
	funcWindowVariance(double length, double pixelSize): funcWindowOpBase(length,pixelSize){}
	
	inline void op(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		safeop(iOp,iRet );
	}

	inline void safeop(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{	
		
		double sum = 0.0;
		double sumsquare = 0.0;
		double tmp = 0;
		int cnt = 0;

		int i = 0;
		int j = 0;
		
		for (i=-_radius; i<=_radius;i++){
			for (j = -_radius; j<=_radius; j++){
				if (iOp.getItemSafe(i,j,tmp)){
					sum+=tmp;
					sumsquare = tmp*tmp;
					cnt++;
				}
			}
		}
		tmp = sum/cnt;

		*iRet = sumsquare/cnt - tmp*tmp; 
	}
};

}
#endif