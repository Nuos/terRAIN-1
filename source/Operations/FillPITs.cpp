#include "FillPITs.h"
#include "WindowIterator.h"
#include "DoubleUtil.h"
#include "Paralellize.h"
#include <algorithm>

namespace TR
{

FillPITs::FillPITs(DblRasterMx & op, DblRasterMx & ret, unsigned int nParts):UnaryMatrixFunc(op,ret)
{
	_nParts = nParts;
}

void FillPITs::run()
{
	(*_pRet) = *_pOp;
	
	unsigned int nPITs = 0;

	if (_nParts>1){	

		typeRegionList safeRegions;
		typeRegionList unsafeRegions;

		createRegions(*_pRet,safeRegions,unsafeRegions);

		std::list<FillPITFunc*> listFunc;

		typeRegionList::iterator iSafeRegions = safeRegions.begin();
		typeRegionList::iterator iSafeEnd	  = safeRegions.end();

		for(;iSafeRegions!=iSafeEnd;iSafeRegions++){

			FillPITFunc * pFunc = new FillPITFunc(iSafeRegions->first,iSafeRegions->second);
			listFunc.push_back(pFunc);
		
		}

		typeRegionList::iterator iUnSafeRegions = unsafeRegions.begin();
		typeRegionList::iterator iUnSafeEnd	    = unsafeRegions.end();
		 
		do{
				
			boost::thread_group		threads;
			
			std::list<FillPITFunc*>::iterator iFunc = listFunc.begin();
			std::list<FillPITFunc*>::iterator iFuncEnd = listFunc.end();

			for (;iFunc!=iFuncEnd;iFunc++){
				threads.create_thread(ThreadFunc(**iFunc));
			}
			
			threads.join_all();

			iFunc = listFunc.begin();

			nPITs = 0;

			for (;iFunc!=iFuncEnd;iFunc++){
				nPITs+=(*iFunc)->getPITNr();
			}

			for (iUnSafeRegions = unsafeRegions.begin(); iUnSafeRegions!=iUnSafeEnd; iUnSafeRegions++){
				
				FillPITFunc fillFITSFunc(iUnSafeRegions->first,iUnSafeRegions->second);
				
				fillFITSFunc.run();

				nPITs+= fillFITSFunc.getPITNr();
			}

		}while(nPITs!=0);

		std::list<FillPITFunc*>::iterator iFunc = listFunc.begin();
		std::list<FillPITFunc*>::iterator iFuncEnd = listFunc.end();

		for (;iFunc!=iFuncEnd;iFunc++){
			delete *iFunc;
		}


	} else if (_nParts==1) {
		
		DblRasterMx::iterator iBegin = _pRet->begin();
		DblRasterMx::iterator iEnd = _pRet->end();

		do{
			FillPITFunc fillPITs( iBegin, iEnd);
			fillPITs.run();

			nPITs = fillPITs.getPITNr();

		} while (nPITs!=0);
		
	}//ERROR!!

}

paralellisable * FillPITs::clone() const
{
	return new FillPITs(*_pOp,*_pRet,_nParts);
}


paralellisable * FillPITFunc::clone()const
{
	FillPITFunc * pFunc = new FillPITFunc(_iBegin,_iEnd);
	
	pFunc->_nPITs = _nPITs;

	return pFunc;

}
void FillPITFunc::run()
{
	_nPITs = 0;

	DblRasterMx mx = _iBegin.getOwner();

	DblRasterMx::iterator iIter;
	
	double endless = DoubleUtil::getMAXValue();

	//minimal neighbour pixel height
	double dblMin1 = endless;
	//second minimal neighbour pixel height
	double dblMin2 = endless;
	
	double dblVal = 0.0;
	unsigned int i =0;
	
	
	
	for (iIter=_iBegin; iIter!=_iEnd; iIter++){
		
		

		//do not fill outflow PITs
		if (iIter.isInOutflowPos())
			continue;

		dblMin1 = endless;
		dblMin2 = endless;

		if (iIter.isOnBound()){
			
			for ( i = 1; i < 10; i++){
				
				if (i==5)
					continue;

				if (iIter.chain_code_safe(i,dblVal)){
					
					if (dblVal<dblMin1){
						dblMin2 = dblMin1;
						dblMin1 = dblVal;
					} else if (dblVal<dblMin2 && !DoubleUtil::cmpD6(dblVal,dblMin1)) {
						dblMin2 = dblVal;
					}

				}

			}
		}else {

			for ( i = 1; i < 10; i++){
				
				if (i==5)
					continue;
				
				dblVal = iIter.chain_code(i);

				if (dblVal<dblMin1){
					dblMin2 = dblMin1;
					dblMin1 = dblVal;
				} else if (dblVal<dblMin2 && !DoubleUtil::cmpD6(dblVal,dblMin1)) {
						dblMin2 = dblVal;
				}

			}
		
		}
		dblVal = *iIter;

		if (dblMin2==endless)
			dblMin2=dblMin1;

		//iIter is a PIT
		if (dblMin1>=dblVal) {
			double delta = !DoubleUtil::cmpD3(dblMin2,dblMin1) ? (dblMin2 - dblMin1)*0.5 :
				(DoubleUtil::cmpD3(dblVal,dblMin1) ? 0.05*dblVal : (dblMin1 - dblVal)*0.5);
			
			if (delta>DoubleUtil::sDeltaD3)
				*iIter = dblMin1 + delta;
			else
				*iIter = dblMin1 + 0.5;

			++_nPITs;
		}

	}
	
	
}

void FillPITs::createRegions(DblRasterMx & matrix,typeRegionList & regions, typeRegionList & criticalRegions)
{
	
	unsigned int nParts = std::min(_nParts, matrix.getRowNr()/5);
	
	typeRegion region;

	if (nParts<2){
		typeRegion region;
		region.first  = matrix.begin();
		region.second = matrix.end();	
    	regions.push_back(region);
		return;
	}
	
	unsigned int nSafeRegionRows = (matrix.getRowNr()/nParts) - 2;
	unsigned int nNextRow = 0;
	unsigned int nLastColIndex = matrix.getColNr()-1;
	
	DblRasterMx::iterator iRegionBegin;
	DblRasterMx::iterator iRegionEnd;

	for ( unsigned int i = 0; i < nParts; i++) {
		
		if (i==0){
			
			region.first  = matrix.begin();
			region.second = matrix.getIteratorAt(nSafeRegionRows+1,nLastColIndex);
			
			regions.push_back(region);

			region.first  = matrix.getIteratorAt(nSafeRegionRows+2,0);
			region.second = matrix.getIteratorAt(nSafeRegionRows+3,nLastColIndex);
			
			criticalRegions.push_back(region);

			nNextRow = nSafeRegionRows+4;
			
		}else if (i==(nParts-1)){
			
			region.first  = matrix.getIteratorAt(nNextRow,0);
			region.second = matrix.end();

			regions.push_back(region);
		
		}else {

			region.first  = matrix.getIteratorAt(nNextRow,0);
			region.second = matrix.getIteratorAt(nNextRow + nSafeRegionRows,nLastColIndex);
			regions.push_back(region);

			region.first  = matrix.getIteratorAt(nNextRow + nSafeRegionRows+1,0);
			region.second = matrix.getIteratorAt(nNextRow + nSafeRegionRows+2,nLastColIndex);
			
			criticalRegions.push_back(region);

			nNextRow = nNextRow + nSafeRegionRows+3;
		
		}
	}




}

}