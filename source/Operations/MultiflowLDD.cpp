#include "MultiflowLDD.h"
#include "defs.h"
#include "DoubleUtil.h"

namespace TR
{
void MultiflowLDD::realMultiflowRun()
{
	DblRasterMx::iterator & iOp			= *_pOpBegin;
	DblRasterMx::iterator & iOpEnd		= *_pOpEnd;
	MultiflowDMatrix::iterator & iRet	= *_pRet;
	
	DblRasterMx::iterator * pMxAIterator = getMxAIterator();

	size_t nRow = 0;
	size_t nCol = 0;

	size_t nLastRowIndex = iRet.getOwner().getRowNr()-1;
	size_t nLastColIndex = iRet.getOwner().getColNr()-1;

	_dblPixelSize = iOp.getOwner().getPixelSize();
	_dblPixelSizeMulSqrt2 = _dblPixelSize * SQRT2;

	double dblCurrentValue = 0.0;
	double dblNoDrainVal   = 0.0;
	double dblNeighbourVal = 0.0;
	double a = 0.0;
	

	for (; iOp!=iOpEnd; ++iOp, ++iRet){
		
		initTempVariables();
        
		if (pMxAIterator!=NULL) {
			a = **pMxAIterator;
			++(*pMxAIterator);
		}else {
			a = _a;
		}

		dblCurrentValue = *iOp;

		nRow = iOp.getRow();
		nCol = iOp.getCol();
				
		if ( nRow==0 || nCol==0 || nRow==nLastRowIndex || nCol==nLastColIndex ){

			for ( unsigned char cc = 1; cc < 10; cc++ ) {
				
				if (cc==5)
					continue;

				if (iOp.chain_code_safe(cc,dblNeighbourVal))
					store(cc,dblCurrentValue,dblNeighbourVal,iRet,a);
				else 
					(*iRet).setByChainCode(cc,dblNoDrainVal);
			}
		
		} else {

			for ( unsigned char cc = 1; cc < 10; cc++ ) {
				
				if (cc==5)
					continue;
				
				store(cc,dblCurrentValue,iOp.chain_code(cc),iRet,a);
			}
		
		}
		
		
		
		
		if (_fillPITS){
			//PIT
			if ( _dblSumDeg < DoubleUtil::sDeltaD6 ){
				if ( !iOp.isInOutflowPos() ){
					(*iRet).setByChainCode(_nMinPixelPos,1.0);	
				}
		
			}else {
				if (!_returnDegs) {
					for ( unsigned int i = 0; i < 8; i++ ) {
						if ( !DoubleUtil::cmpD6((*iRet)(i),0.0) )
							(*iRet)(i)/=_dblSumDeg;
					}
				}
			}
		}else {
			if (!_returnDegs) {
				for ( unsigned int i = 0; i < 8; i++ ) {
					if ( !DoubleUtil::cmpD6((*iRet)(i),0.0) )
						(*iRet)(i)/=_dblSumDeg;
				}
			}
		}
		
		
	}
}

void MultiflowLDD::semiMultiflowRun()
{
	DblRasterMx::iterator & iOp			= *_pOpBegin;
	DblRasterMx::iterator & iOpEnd		= *_pOpEnd;
	MultiflowDMatrix::iterator & iRet	= *_pRet;
	
	
	_dblPixelSize = iOp.getOwner().getPixelSize();
	_dblPixelSizeMulSqrt2 = _dblPixelSize * SQRT2;

	double dblNoDrainVal   = 0.0;
	double dblNeighbourVal = 0.0;
	
	double dblTmp = 0.0;

	for (; iOp!=iOpEnd; ++iOp, ++iRet){

		double dblCurrentValue = *iOp;
		
		double maxVal =0;
		unsigned char maxPos = 0;
		
		for ( unsigned char cc = 1; cc < 10; cc++ ) {
			
			
			if (cc==5)
				continue;
			
			(*iRet).setByChainCode(cc,0.0);

			if (iOp.chain_code_safe(cc,dblNeighbourVal)){
				
				if (dblNeighbourVal>=dblCurrentValue)
					continue;

				if (cc==2 || cc==4 || cc==6 || cc==8 ) 
					dblTmp = atan((dblCurrentValue - dblNeighbourVal)/_dblPixelSize);
				else
					dblTmp = atan((dblCurrentValue - dblNeighbourVal)/_dblPixelSizeMulSqrt2);

				if (dblTmp>maxVal){
					maxVal = dblTmp;
					maxPos = cc;
				}
			}
		}

		if (maxPos>0){
			(*iRet).setByChainCode(maxPos,1.0);	
		}
	}

}

void MultiflowLDD::run()
{
	if (_singleFlowMode)
		semiMultiflowRun();
	else
		realMultiflowRun();
}



paralellisable * MultiflowLDD::clone() const
{
	return new MultiflowLDD(_a,*_pOpBegin,*_pOpEnd,*_pRet,_fillPITS,_singleFlowMode, _returnDegs);
}

DblRasterMx::iterator & MultiflowLDD::getOpBegin()
{
	return *_pOpBegin;
}

DblRasterMx::iterator & MultiflowLDD::getOpEnd()
{
	return *_pOpEnd;
}

DblRasterMx::iterator * MultiflowLDD::getMxAIterator()
{
	return _pMxA;
}

MultiflowDMatrix::iterator & MultiflowLDD::getRetIterator()
{
	return *_pRet;
}

void MultiflowLDD::setOpBegin(DblRasterMx::iterator & iOpBegin)
{
	_pOpBegin = & iOpBegin;
}

void MultiflowLDD::setOpEnd(DblRasterMx::iterator & iOpEnd)
{
	_pOpEnd = & iOpEnd;
}

void MultiflowLDD::setRetIterator(MultiflowDMatrix::iterator & iRet)
{
	_pRet = & iRet;
}

}