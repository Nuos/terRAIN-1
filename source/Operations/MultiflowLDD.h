#ifndef MULTIFLOWLDD_H
#define MULTIFLOWLDD_H

#include "paralellisable.h"
#include "MultiflowDMatrix.h"

namespace TR
{

class MultiflowLDD : public paralellisable
{
protected:
	
	double _a;
	DblRasterMx::iterator * _pOpBegin;
	DblRasterMx::iterator * _pOpEnd;
	MultiflowDMatrix::iterator * _pRet;
	DblRasterMx::iterator * _pMxA;

	double _dblSumDeg;
	double _dblMinPixelVal;
	unsigned char _nMinPixelPos;
	double _dblPixelSize;
	double _dblPixelSizeMulSqrt2;
	bool _fillPITS;
	bool _singleFlowMode;
public:
	
	MultiflowLDD(double a, DblRasterMx::iterator & iOpBegin, DblRasterMx::iterator & iOpEnd, MultiflowDMatrix::iterator & iRet, bool fillPITS, bool singleFlow):
		_a(a),_pOpBegin(&iOpBegin), _pOpEnd(&iOpEnd), _pRet(&iRet),_dblPixelSize(1.0),_dblPixelSizeMulSqrt2(1.0),_fillPITS(fillPITS),_singleFlowMode(singleFlow),
		_pMxA(NULL)
	{
		initTempVariables();
	}

	MultiflowLDD(DblRasterMx::iterator & iOpBegin, DblRasterMx::iterator & iOpEnd, MultiflowDMatrix::iterator & iRet, bool fillPITS, bool singleFlow, DblRasterMx::iterator & iMxA):
		_a(0.0),_pOpBegin(&iOpBegin), _pOpEnd(&iOpEnd), _pRet(&iRet),_dblPixelSize(1.0),_dblPixelSizeMulSqrt2(1.0),_fillPITS(fillPITS),_singleFlowMode(singleFlow),
		_pMxA(&iMxA)
	{
		initTempVariables();
	}

	virtual void run();
	virtual paralellisable * clone() const;

	DblRasterMx::iterator & getOpBegin();
	DblRasterMx::iterator & getOpEnd();
	MultiflowDMatrix::iterator & getRetIterator();
	DblRasterMx::iterator * getMxAIterator();

	void setOpBegin(DblRasterMx::iterator & iOpBegin);
	void setOpEnd(DblRasterMx::iterator & iOpEnd);
	void setRetIterator(MultiflowDMatrix::iterator & iRet);

	inline void initTempVariables();
	inline void store(unsigned char nChainCode, double dblCurrentVal, double dblNeighbourVal, MultiflowDMatrix::iterator & iRet, double a);

private:

	void realMultiflowRun();
	void semiMultiflowRun();

};

void MultiflowLDD::initTempVariables()
{
	_dblSumDeg			= 0.0;
	_dblMinPixelVal		= 0.0;
	// set it to invalid value to signal, _dblMinPixelVal and _nMinPixelPos are uninitalized
	_nMinPixelPos		= 10;
}

inline void MultiflowLDD::store(unsigned char nChainCode, double dblCurrentVal, double dblNeighbourVal, MultiflowDMatrix::iterator & iRet, double a)
{
	
	if ( _nMinPixelPos>9 || (dblNeighbourVal < _dblMinPixelVal)){
		_nMinPixelPos	= nChainCode;
		_dblMinPixelVal = dblNeighbourVal;
	}
	
	double dblTmp = 0.0;

	if ( dblNeighbourVal < dblCurrentVal ) {
		
		if (nChainCode==2 || nChainCode==4 || nChainCode==6 || nChainCode==8 ) 
			dblTmp = atan((dblCurrentVal - dblNeighbourVal)/_dblPixelSize);
		else
			dblTmp = atan((dblCurrentVal - dblNeighbourVal)/_dblPixelSizeMulSqrt2);
		
		dblTmp = std::pow(dblTmp,a);
		_dblSumDeg+= dblTmp;
		(*iRet).setByChainCode(nChainCode,dblTmp);

	}else
		(*iRet).setByChainCode(nChainCode,0.0);


}

}
#endif