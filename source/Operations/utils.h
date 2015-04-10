#ifndef UTILS_H
#define UTILS_H

#include "defs.h"
#include "DoubleUtil.h"
#include "MultiflowDMatrix.h"

namespace TR
{

inline double normalizeRad( double dblRad );
inline unsigned char myChainCode( unsigned char nChainCode );
inline bool isRidgePoint( MultiflowDMatrix::iterator & iIter );
inline bool isRidgePoint( DblRasterMx::iterator & iIter );
inline unsigned short nextChainCode( unsigned short nChainCode );
inline bool isPIT(const MultiflowDMatrix::iterator & iterator);


bool isRidgePoint( DblRasterMx::iterator & iIter )
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

bool isRidgePoint( MultiflowDMatrix::iterator & iIter )
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

double normalizeRad( double dblRad )
{
	if (dblRad>=0.0 && dblRad < 2*M_PI)
		return dblRad;

	/*
	if (std::fabs(dblRad)>2*M_PI){
		
		int n = dblRad/(2*M_PI);

		dblRad-= n

	}
	*/

	while ( std::fabs(dblRad)>2*M_PI ) {
		
		if (dblRad>0.0)
			dblRad-=2*M_PI;
		else
			dblRad+=2*M_PI;

	}

	if ( dblRad < 0.0 )
		dblRad+=2*M_PI;

	return dblRad;
}

unsigned char myChainCode( unsigned char nMyChainCode )
{
	switch ( nMyChainCode ) {
		case 1:
			return 9;
		case 2:
			return 8;
		case 3:
			return 7;
		case 4:
			return 6;
		case 5:
			return 5;
		case 6:
			return 4;
		case 7:
			return 3;
		case 8:
			return 2;
		case 9:
			return 1;
		break;
	}

	// throw exception: invalid chain code
	return 0;
}

unsigned short nextChainCode( unsigned short nChainCode )
{
	switch ( nChainCode ) {
		case 1:
			return 2;
		case 2:
			return 3;
		case 3:
			return 6;
		case 4:
			return 1;
		case 6:
			return 9;
		case 7:
			return 4;
		case 8:
			return 7;
		case 9:
			return 8;
		break;
	}

	// throw exception: invalid chain code
	return 0;
}

inline bool isPIT(const MultiflowDMatrix::iterator & iterator)
{

	for (unsigned int i = 0; i < 8; i++){
		if ( (*iterator)(i)>DoubleUtil::sDeltaD6 )
			return false;
	}

	return true;	
}

}

#endif