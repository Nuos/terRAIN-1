#include "FillMLDDPits.h"
#include "utils.h"
#include "rastermatrix.h"
#include "MLDDFunctions.h"
#include "ConstantMarkFunction.h"
#include "MarkTouchedRasters.h"
#include <list>

using namespace std;

namespace TR
{

FillMLDDPits::FillMLDDPits(MultiflowDMatrix & mxMLDD, MultiflowDMatrix & mxRet):
	_mxMLDD(mxMLDD), _mxRet(mxRet)
{}


void FillMLDDPits::run()
{
	size_t rows = _mxMLDD.getRowNr();
	size_t cols = _mxMLDD.getColNr();
	
	_mxRet = _mxMLDD;
	double initVal = 0.0;

	//DblRasterMx mx;
	mx.init(rows,cols,0.0,TR::origoBottomLeft,initVal);
	
	MultiflowDMatrix::iterator iter = _mxMLDD.begin();
	MultiflowDMatrix::iterator end = _mxMLDD.end();
	
	MarkTouchedRasters markFunc(mx, 1.0);
	
	std::list<MultiflowDMatrix::iterator> maxPoints;
	std::list<MultiflowDMatrix::iterator> minPoints;
	

	for(; iter!=end; ++iter){
		if (iter.isInOutflowPos()){
			MLDDFunctions::walkBackOnFlow(iter,markFunc,maxPoints);
		}
	}

	size_t pixelCnt = rows*cols;
	
	std::list<MultiflowDMatrix::iterator>::iterator it, e;

	while (markFunc.getCnt()<pixelCnt && !maxPoints.empty()){
		
		markFunc.setMark(-1.0);
		
		it = maxPoints.begin();
		e = maxPoints.end();

		for (; it!=e; ++it){
			MLDDFunctions::walkOnFlow(*it,markFunc,minPoints);
		}
		
		maxPoints.clear();
		markFunc.setMark(1.0);

		it = minPoints.begin();
		e = minPoints.end();

		for (; it!=e; ++it){
			MLDDFunctions::walkBackOnFlow(*it,markFunc,maxPoints);
		}

		minPoints.clear();
	}
	
	invertNotMarkedRasters(_mxMLDD,_mxRet,mx);
}


void FillMLDDPits::invertNotMarkedRasters(MultiflowDMatrix & mxSource,MultiflowDMatrix & mxTarget, DblRasterMx & mx) 
{
	DblRasterMx::iterator iter = mx.begin();
	DblRasterMx::iterator end = mx.end();
	
	size_t row = 0;
    size_t col = 0;
	
	unsigned char cc = 0;
	double noDrain = 0.0;
	for (; iter!=end; ++iter){
		
		if (*iter==-1.0){

			row = iter.getRow();
			col = iter.getCol();

			MultiflowDMatrix::iterator it = mxSource.getIteratorAt(row,col);
			
			if (isPIT(it))
				continue;

			for (cc=1; cc < 10; cc++){
			
				if (cc==5)
					continue;
			
				MultiflowDMatrix::iterator neighbour;

				if (it.neighbourIterator(cc,neighbour)){
					unsigned char mychainCode = myChainCode(cc);
					double val = (*neighbour).getByChainCode(mychainCode);
					
					if (val>DoubleUtil::sDeltaD6){
						mxTarget(row,col).setByChainCode(cc,(*neighbour).getByChainCode(mychainCode));
						mxTarget(neighbour.getRow(),neighbour.getCol()).setByChainCode(mychainCode,noDrain);
					}
					
					val = (*it).getByChainCode(cc);
					
					if (val>DoubleUtil::sDeltaD6){
						mxTarget(row,col).setByChainCode(cc,noDrain);
						mxTarget(neighbour.getRow(),neighbour.getCol()).setByChainCode(mychainCode,val);
					}


				}
			}	
		}
	}

}
// walk back from iter to ridge points and mark the points in mx with 1.0
void FillMLDDPits::markAvailableRasters(MultiflowDMatrix::iterator iter, DblRasterMx & mx, double sign) const
{
	std::list<MultiflowDMatrix::iterator> checkList;

	size_t row = iter.getRow();
	size_t col = iter.getCol();

	mx(row,col) = sign;
	
	unsigned char cc = 0;
	
	checkList.push_back(iter);

	while (!checkList.empty()) {
		
		MultiflowDMatrix::iterator  it = checkList.back();
		checkList.pop_back();
	
		for (cc=1; cc < 10; cc++){
			
			if (cc==5)
				continue;
			
			MultiflowDMatrix::iterator neighbour;
			

			if (it.neighbourIterator(cc,neighbour)){

				if ((*neighbour).getByChainCode(myChainCode(cc))>DoubleUtil::sDeltaD6){
					
					row = neighbour.getRow();
					col = neighbour.getCol();
				
					if (mx(row,col)==0.0){
						mx(row,col)+=sign;
						checkList.push_back(neighbour);
					}
				}
			}
		}
	
	}
}

paralellisable * FillMLDDPits::clone() const
{
	return new FillMLDDPits(_mxMLDD,_mxRet);
}

}