#include "MLDDFunctions.h"
#include "rastermatrix.h"
#include "utils.h"
#include <list>

namespace TR
{



void MLDDFunctions::walkBackOnFlow(MultiflowDMatrix::iterator iter, MLDDWalkFunction & func, std::list<MultiflowDMatrix::iterator> & maxPoints)
{
	std::list<MultiflowDMatrix::iterator> checkList;

	func(iter);
	
	unsigned char cc = 0;
	
	checkList.push_back(iter);

	while (!checkList.empty()) {
		
		MultiflowDMatrix::iterator  it = checkList.back();
		checkList.pop_back();	
		
		size_t cnt = 0;

		for (cc=1; cc < 10; cc++){
			
			if (cc==5)
				continue;
			
			if (it->getByChainCode(cc)>DoubleUtil::sDeltaD6)
				continue;

			MultiflowDMatrix::iterator neighbour;
			
			if (it.neighbourIterator(cc,neighbour)){

				if ((*neighbour).getByChainCode(myChainCode(cc))>DoubleUtil::sDeltaD6){
					
					if (func(neighbour)){
						checkList.push_back(neighbour);
						++cnt;
					}
		
				}
			}
		}

		if (cnt==0){
			maxPoints.push_back(it);
		}
	
	}
}

void MLDDFunctions::walkOnFlow(MultiflowDMatrix::iterator iter, MLDDWalkFunction & func, std::list<MultiflowDMatrix::iterator> & minPoints)
{
	std::list<MultiflowDMatrix::iterator> checkList;

	//func(iter);
	
	unsigned char cc = 0;
	
	checkList.push_back(iter);

	while (!checkList.empty()) {
		
		MultiflowDMatrix::iterator  it = checkList.back();
		checkList.pop_back();	
		
		size_t cnt = 0;

		for (cc=1; cc < 10; cc++){
			
			if (cc==5)
				continue;
			
			if (it->getByChainCode(cc)<DoubleUtil::sDeltaD6)
				continue;

			MultiflowDMatrix::iterator neighbour;
			
			if (it.neighbourIterator(cc,neighbour)){

				if (func(neighbour)){
						checkList.push_back(neighbour);
						++cnt;
				}
			}
		}

		if (cnt==0){
			minPoints.push_back(it);
		}
	}
}

}


