#include "accfluxfunctions.h"
#include "utils.h"
#include "DoubleUtil.h"
#include <list>
#include "ItemDescriptor.h"
#include <fstream>

using namespace TR;

namespace FUNC
{
/*
bool funcAccFluxOnLDD::isRidgePoint( DblRasterMx::iterator & iIter ) const
{
	
	unsigned char cc = 0;

	if (iIter.isOnBound()){
		
		double dblTmp = 0.0;

		for (cc=1; cc < 10; cc++){
			
			if (cc==5)
				continue;

			if (iIter.chain_code_safe(cc,dblTmp)){
				if (static_cast<unsigned char>(dblTmp)==myChainCode(cc))
					return false;
			}
		}

		return true;
	}else {

		for (cc=1; cc < 10; cc++){
			
			if (cc==5)
				continue;
			
			if ( iIter.chain_code(cc)==myChainCode(cc) )
				return false;
		}

		return true;
	
	}
}
*/

void funcAccFluxOnLDD::processPoint( DblRasterMx::iterator & iIter )
{
	/*
	DblRasterMx::iterator iLDD = iIter;
	recursiveAccFluxCall(iLDD,0.0);
	*/

	nonRecursiveAccFluxCall(iIter);
}

void funcAccFluxOnLDD::nonRecursiveAccFluxCall( DblRasterMx::iterator & iLDD)
{
	size_t	nRow = iLDD.getRow();
	size_t	nCol = iLDD.getCol();

	double dblValue = 0.0;
	size_t nIterCnt	= 0;
	
	unsigned char nChainCode = 0;;

	DblRasterMx::iterator iTmp = iLDD;
	dblValue=_mxDepth(nRow,nCol);
	while (nIterCnt<_nMaxIter) {
		
		//dblValue+=_mxDepth(nRow,nCol);
		_mxRet(nRow,nCol)+= dblValue;
		
		nChainCode = static_cast<unsigned char>(*iTmp);

		if ( nChainCode==5)
			return;
		

		iTmp.skipToChainCode(nChainCode);

		nRow = iTmp.getRow();
		nCol = iTmp.getCol();
		++nIterCnt;
	}
	
	std::cout<<"ehh";

}
void funcAccFluxOnLDD::recursiveAccFluxCall( DblRasterMx::iterator & iLDD, double dblVal)
{
	size_t	nRow = iLDD.getRow();
	size_t	nCol = iLDD.getCol();

	dblVal+=_mxDepth(nRow,nCol);
	_mxRet(nRow,nCol) = dblVal;
	
	unsigned char nNextChainCode = static_cast<unsigned char>(*iLDD);
	
	if (nNextChainCode==5)
		return;

	iLDD.skipToChainCode(nNextChainCode);
	
	recursiveAccFluxCall(iLDD,dblVal);
	

}
/*
bool funcAccFluxOnMLDD::isRidgePoint( MultiflowDMatrix::iterator & iIter ) const
{
	unsigned char cc = 0;
	int nNotZeroDrain = 0;

	if (iIter.isOnBound()){
		
		DoubleChainCodeData dataChainCode;

		for (cc=1; cc < 10; cc++){
			
			if (cc==5)
				continue;

			if (iIter.isValidItemByChainCode(cc)){
				
				if (iIter.chain_code(cc).getByChainCode(myChainCode(cc))>DoubleUtil::sDeltaD6 ) 
					nNotZeroDrain++;
			}
		}

	}else {

		for (cc=1; cc < 10; cc++){
			
			if (cc==5)
				continue;
			
			if (iIter.chain_code(cc).getByChainCode(myChainCode(cc))>DoubleUtil::sDeltaD6 ) 
				nNotZeroDrain++;
		}
	}

	return nNotZeroDrain==0;
}
*/

void funcAccFluxOnMLDD::processPoint( MultiflowDMatrix::iterator & iIter )
{
	std::list<ItemDescriptor> listDescriptors;
	size_t nMaxDepth = 0;

	size_t nRow = iIter.getRow();
	size_t nCol = iIter.getCol();

	ItemDescriptor root(iIter,_mxDepth(nRow,nCol));
	ItemDescriptor descTmp(root);
	
	double dblFluxLimit = root._dblValue*_dblFluxLimitRate;
	

	listDescriptors.push_back(root);
	unsigned int nCnt = 0;
	double dblVal	 = 0.0;
	bool bFound = false;
	while ( !listDescriptors.empty() ) {
		
		ItemDescriptor & currentDesc = listDescriptors.back();
		
		DoubleChainCodeData & chainData = *currentDesc._iLDD;

		bFound = false;
		while (currentDesc._nChainCode<10){
			
			dblVal = chainData.getByChainCode(currentDesc._nChainCode);
			
			if (dblVal>dblFluxLimit) {
				
				bFound = true;
				descTmp = currentDesc;
				descTmp._nChainCode = 1;
				descTmp._iLDD.skipToChainCode(currentDesc._nChainCode);
				
				nRow = descTmp._iLDD.getRow();
				nCol = descTmp._iLDD.getCol();
				
				descTmp._dblValue = dblVal*currentDesc._dblValue;
				_mxRet(nRow,nCol) += descTmp._dblValue; 
				listDescriptors.push_back(descTmp);
			}

			currentDesc.incChainCode();		
		}


		if (!bFound)
			listDescriptors.pop_back();

		++nCnt;
	}

}


void funcLoopCheck::processPoint( DblRasterMx::iterator & iIter )
{
	call(iIter);
}

bool funcLoopCheck::call( DblRasterMx::iterator & iLDD)
{
	size_t	nRow = iLDD.getRow();
	size_t	nCol = iLDD.getCol();

	double dblValue = 0.0;
	size_t nIterCnt	= 0;
	
	unsigned char nChainCode = 0;;

	DblRasterMx::iterator iTmp = iLDD;
	
	while (nIterCnt<_nMaxIter) {
		
		_mxRet(nRow,nCol)+= 1;
		
		nChainCode = static_cast<unsigned char>(*iTmp);

		if ( nChainCode==5)
			return false;

		iTmp.skipToChainCode(nChainCode);

		nRow = iTmp.getRow();
		nCol = iTmp.getCol();
		++nIterCnt;
	}

	std::cout<<"loop"<<std::endl;

	return true;
}

}