#include "spreadLDD.h"
#include "rastermatrix.h"
#include "MultiflowDMatrix.h"
#include "utils.h"
#include <list>
#include <algorithm>

namespace TR {

	SpreadLDD::SpreadLDD(const MultiflowDMatrix & mxMLDD, const DblRasterMx & mxPoints, DblRasterMx & mxRet, double dblFluxLimitRate):
	_mxMLDD(mxMLDD), _mxPoints (mxPoints), _mxRet(mxRet), _dblFluxLimitRate(dblFluxLimitRate)
	{
		_pixelSize = _mxPoints.getPixelSize();
	}
	
	void SpreadLDD::run()
	{
		_mxRet.initlike(_mxPoints);
		_mxRet.fill(DoubleUtil::getNAN());

		MultiflowDMatrix::iterator iter;
		MultiflowDMatrix::iterator end =  _mxMLDD.end();
     
		for (iter = _mxMLDD.begin(); iter!=end; ++iter){
			if (isRidgePoint(iter)){
				processRidgePoint(iter);
			}
		}
		
		DblRasterMx::iterator iRet = _mxRet.begin();
		DblRasterMx::iterator iRetEnd = _mxRet.end();

		for (; iRet!=iRetEnd; ++iRet) {
			if (DoubleUtil::isNAN(*iRet)) {
				std::cout << "Error in spreadLDD: Undefined pixel value in: (" << iRet.getRow() << "," << iRet.getCol() << ")" << std::endl;
				*iRet=-1.0;
			} else if ( *iRet < 0.0 ) {
				*iRet = -1.0;
			}
		}

	}

	void SpreadLDD::processRidgePoint( MultiflowDMatrix::iterator & item)
	{
		size_t nRow = item.getRow();
		size_t nCol = item.getCol();
		
		std::list<MultiflowDMatrix::iterator> listTempData;
		listTempData.push_back(item);
		
		while (!listTempData.empty()) {
			MultiflowDMatrix::iterator iter = listTempData.back();
			listTempData.pop_back();

			size_t nIterRow = iter.getRow();
			size_t nIterCol = iter.getCol();

			if (!DoubleUtil::isNAN(_mxRet(nIterRow,nIterCol))) {
                continue;
			}

			bool bAllParentIsFinished = true;
			unsigned char cc = 1;
			double dblMinOfNonNegativeParents = DoubleUtil::infinite();
			double dblMinOfNegativeparents = 0.0;
			unsigned char ccMinOfNonNegativeParents = 5;
			for ( cc = 1; cc < 10; cc++){

				if (cc!=5 && iter.isValidItemByChainCode(cc)) {
					
					MultiflowDMatrix::iterator iterNeighbour(iter);
					iterNeighbour.skipToChainCode(cc);

					unsigned char nMyChainCode = myChainCode(cc);
					DoubleChainCodeData & chainData = *iterNeighbour;

					double dblDrainRate = chainData.getByChainCode(nMyChainCode);

					if (dblDrainRate>_dblFluxLimitRate){
						
						nRow = iterNeighbour.getRow();
						nCol = iterNeighbour.getCol();
						
						double parentVal = _mxRet(nRow,nCol); 
						if (DoubleUtil::isNAN(parentVal)){
							bAllParentIsFinished = false;
							break;
						}
						
						if (parentVal < 0.0) {
							dblMinOfNegativeparents = std::min( dblMinOfNegativeparents, parentVal);	
						} else {
							if (parentVal<dblMinOfNonNegativeParents) {
								dblMinOfNonNegativeParents = parentVal;
								ccMinOfNonNegativeParents = cc;
							}
						}
					}
				
				}
			}

			if (bAllParentIsFinished) {

				if (_mxPoints(nIterRow, nIterCol) > 0) {
					_mxRet(nIterRow,nIterCol)=0.0;
				} else if ( !DoubleUtil::isInfinite(dblMinOfNonNegativeParents)) {
					
					double inc = 1.0;
					if (ccMinOfNonNegativeParents==1 || ccMinOfNonNegativeParents==3 ||
						ccMinOfNonNegativeParents==7 || ccMinOfNonNegativeParents==9) {
						inc = SQRT2;
					}

					_mxRet(nIterRow,nIterCol) = dblMinOfNonNegativeParents + inc*_pixelSize;
				} else 
					_mxRet(nIterRow,nIterCol) = dblMinOfNegativeparents - 1.0;

				DoubleChainCodeData & chainData = *iter;

				for ( cc = 1; cc < 10; cc++){
					if (cc!=5 && chainData.getByChainCode(cc)>0.0){
						listTempData.push_back(iter);
						listTempData.back().skipToChainCode(cc);
					}
				}
			 }
		}

	}
}