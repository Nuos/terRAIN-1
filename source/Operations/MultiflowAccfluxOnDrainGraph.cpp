#include "MultiflowAccfluxOnDrainGraph.h"
#include "utils.h"

#include <list>

namespace TR {

MultiflowAccfluxOnDrainGraph::MultiflowAccfluxOnDrainGraph(MultiflowDMatrix & mfLDD, DblRasterMx & mxDepth,DblRasterMx & retAccflux,double dblFluxLimitRate)
: _mfLDD(mfLDD),_mxDepth(mxDepth),_accflux(retAccflux), _dblFluxLimitRate(dblFluxLimitRate)
{
	size_t nRows = mfLDD.getRowNr();
	size_t nCols = mfLDD.getColNr();

	if (nCols>0 && nRows>0)
		_accFluxData.init(nRows,nCols);
	
}

void MultiflowAccfluxOnDrainGraph::run()
{
	MultiflowDMatrix::iterator iter;
	MultiflowDMatrix::iterator end = _mfLDD.end();

	for (iter = _mfLDD.begin(); iter!=end; ++iter){
		if (isRidgePoint(iter)){
			processRidgePoint(iter);
		}
	}

	_accflux.initlike(_mxDepth);
	
	size_t nRows = _accFluxData.getRowNr();
	size_t nCols = _accFluxData.getColNr();

	for ( size_t i = 0; i < nRows; i++ ){
		for ( size_t j = 0; j < nCols; j++ ) {
			_accflux(i,j) = _accFluxData(i,j).getValue();		
		}
	}
	
}

void MultiflowAccfluxOnDrainGraph::processRidgePoint( MultiflowDMatrix::iterator & item)
{
	std::list<MultiflowDMatrix::iterator> listTempData;

	size_t nRow = 0;
	size_t nCol = 0;
	unsigned char cc = 1;
	 
	listTempData.push_back(item);

	while ( !listTempData.empty() ) {
		
	
		MultiflowDMatrix::iterator  iter = listTempData.back();	
		listTempData.pop_back();
		
		if ( _accFluxData(iter.getRow(),iter.getCol()).isFinished() )
			continue;

		double sumOfFlux = 0.0;
		bool bAllParentIsFinished = true;

		for ( cc = 1; cc < 10; cc++){

			if (cc!=5 && iter.isValidItemByChainCode(cc)){
				
				MultiflowDMatrix::iterator iterNeighbour(iter);
				iterNeighbour.skipToChainCode(cc);

				unsigned char nMyChainCode = myChainCode(cc);
				DoubleChainCodeData & chainData = *iterNeighbour;

				double dblDrainRate = chainData.getByChainCode(nMyChainCode);

				if (dblDrainRate>_dblFluxLimitRate){
					
					nRow = iterNeighbour.getRow();
					nCol = iterNeighbour.getCol();

					if (!_accFluxData(nRow,nCol).isFinished()){
						bAllParentIsFinished = false;
						break;
					}

					sumOfFlux+= dblDrainRate*_accFluxData(nRow,nCol).getValue();
				}
			
			}
		}

		if (bAllParentIsFinished){

			nRow = iter.getRow();
			nCol = iter.getCol();

			_accFluxData(nRow,nCol).setValue(sumOfFlux + _mxDepth(nRow,nCol));

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