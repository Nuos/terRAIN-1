#include "longestFlowPathLengthFunctors.h"
#include <list>
#include "ItemDescriptor.h"
#include "DoubleUtil.h"
namespace FUNC
{

void funcLongestFlowPathLengthLDD::processRidgePoint( DblRasterMx::iterator & iLDD )
{
	size_t	nRow = iLDD.getRow();
	size_t	nCol = iLDD.getCol();

	double dblValue = 0.0;
	size_t nIterCnt		= 0;
	
	unsigned char nChainCode = 0;;

	DblRasterMx::iterator iTmp = iLDD;

	while (nIterCnt<_nMaxIter) {
		
		_mxRet(nRow,nCol) = std::max(dblValue,_mxRet(nRow,nCol));

		if (_mxRet(nRow,nCol)>dblValue)
			dblValue = _mxRet(nRow,nCol);	
		else
			_mxRet(nRow,nCol) = dblValue;
		
		nChainCode = static_cast<unsigned char>(*iTmp);

		if (nChainCode==1 || nChainCode==3 || nChainCode==7 || nChainCode==9)
			dblValue+=_dblPixelSize*SQRT2;
		else
			dblValue+=_dblPixelSize;

		if ( nChainCode==5)
			return;

		iTmp.skipToChainCode(nChainCode);
		
		nRow = iTmp.getRow();
		nCol = iTmp.getCol();
		++nIterCnt;
	}

	//error, cycle in the graph!!
}


void funcLongestFlowPathLengthMLDD::processRidgePoint( MultiflowDMatrix::iterator & iLDD )
{

	std::list<ItemDescriptor> listDescriptors;
	size_t nMaxDepth = 0;

	size_t nRow = iLDD.getRow();
	size_t nCol = iLDD.getCol();

	ItemDescriptor root(iLDD,0.0);
	ItemDescriptor descTmp(root);
	
	_mxRet(nRow,nCol) = root._dblValue;

	listDescriptors.push_back(root);

	unsigned char cc = 0;
	double dblVal	 = 0.0;
	bool bFound = false;
	while ( !listDescriptors.empty() ) {
		
		ItemDescriptor & currentDesc = listDescriptors.back();
		
		DoubleChainCodeData & chainData = *currentDesc._iLDD;
		bFound = false;
		while (currentDesc._nChainCode<10){
			
			dblVal = chainData.getByChainCode(currentDesc._nChainCode);

			if (dblVal>DoubleUtil::sDeltaD6) {
				
				bFound = true;
				descTmp = currentDesc;
				descTmp._nChainCode = 1;
				descTmp._iLDD.skipToChainCode(currentDesc._nChainCode);
				
				nRow = descTmp._iLDD.getRow();
				nCol = descTmp._iLDD.getCol();

				if (currentDesc._nChainCode==1 || currentDesc._nChainCode==3 || currentDesc._nChainCode==7 || currentDesc._nChainCode==9)
					descTmp._dblValue+=_dblPixelSize*SQRT2;
				else
					descTmp._dblValue+=_dblPixelSize;

				if ( _mxRet(nRow,nCol)>descTmp._dblValue )
					descTmp._dblValue = _mxRet(nRow,nCol);
				else
					_mxRet(nRow,nCol) = descTmp._dblValue;

				listDescriptors.push_back(descTmp);
			}

			currentDesc.incChainCode();
		}

		if (!bFound)
			listDescriptors.pop_back();
	}

}

}

