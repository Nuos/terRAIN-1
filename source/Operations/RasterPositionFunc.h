#ifndef RASTERPOSITIONMATRIXFUNC_H
#define RASTERPOSITIONMATRIXFUNC_H

#include "RasterPositionMatrix.h"

namespace TR
{

class RasterPositionFunc : public paralellisable
{

protected:
	RasterPositionMatrix::iterator * _iOpBegin;
	RasterPositionMatrix::iterator * _iOpEnd;
	DblRasterMx::iterator		   * _iRet;
public:
	RasterPositionFunc( RasterPositionMatrix::iterator & iOpBegin, RasterPositionMatrix::iterator & iOpEnd, DblRasterMx::iterator & iRet):
      _iOpBegin(&iOpBegin), _iOpEnd(&iOpEnd), _iRet(&iRet)
	{}

	  RasterPositionMatrix::iterator & getOpBegin() {
		  return * _iOpBegin;
	  }

	  RasterPositionMatrix::iterator & getOpEnd() {
		  return * _iOpEnd;
	  }

	  DblRasterMx::iterator & getRetBegin() {
		  return *_iRet;
	  }

	  void setOpBegin( RasterPositionMatrix::iterator & iOpBegin ) {
		_iOpBegin = &iOpBegin;
	  }

	  void setOpEnd( RasterPositionMatrix::iterator & iOpEnd ) {
		_iOpEnd = &iOpEnd;
	  }

	  void setRetBegin(DblRasterMx::iterator & iRet) {
		_iRet = &iRet;
	  }
};

}


#endif