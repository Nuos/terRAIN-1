#ifndef STREAMFUNCTORS_H
#define STREAMFUNCTORS_H

#include "MultiflowDMatrix.h"
#include "utils.h"
#include "RasterPositionMatrix.h"

namespace FUNC
{

class dummyStreamFunc
{
public:
	inline double op(DblRasterMx::iterator & iLDD, DblRasterMx::iterator & iDepth, RasterPositionMatrix::iterator & iPos)
	{
		return *iDepth;
	}

	inline double safeop(DblRasterMx::iterator & iLDD, DblRasterMx::iterator & iDepth, RasterPositionMatrix::iterator & iPos)
	{
		return *iDepth;
	}
};

class funcDownStreamBaseLDD
{

};

class funcDownStreamBaseMLDD
{
protected:
	bool _inFlow;
public:
	funcDownStreamBaseMLDD(bool inFlow) : _inFlow(inFlow){}
};


class funcUpStreamBaseLDD
{
protected:
	bool isStreamFrom( DblRasterMx::iterator & iLDD, unsigned char nChainCode ) const
	{
		return (iLDD.chain_code(nChainCode)==myChainCode(nChainCode));
	}
};

class funcUpStreamBaseMLDD
{
protected:
	bool _inFlow;
	
	funcUpStreamBaseMLDD(bool inFlow):_inFlow(inFlow){}

	double getLDDFrom(MultiflowDMatrix::iterator & iLDD, unsigned char nChainCode) const
	{
		return iLDD.chain_code(nChainCode).getByChainCode(myChainCode(nChainCode));
	}
};

class funcDownStreamTotalLDD: public funcDownStreamBaseLDD
{
public:
	
	inline double op(DblRasterMx::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		return *iLDD==5 ? 0.0 : iDepth.chain_code(static_cast<unsigned char>(*iLDD));
	}

	inline double safeop(DblRasterMx::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		return op(iLDD,iDepth,iPos);
	}
};

class funcDownStreamTotalMLDD: public funcDownStreamBaseMLDD
{

public:
	
	funcDownStreamTotalMLDD() : funcDownStreamBaseMLDD(false){}

	inline double op(MultiflowDMatrix::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		double dblStream = 0.0;

		for ( unsigned char cc = 1; cc<10; cc++) {
			
			if (cc==5)
				continue;

			if ((*iLDD).getByChainCode(cc)>DoubleUtil::sDeltaD6)
				dblStream+= iDepth.chain_code(cc);
		
		}

		return dblStream;
	}

	inline double safeop(MultiflowDMatrix::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		return op(iLDD,iDepth,iPos);
	}
};

class funcUpStreamTotalLDD: public funcUpStreamBaseLDD
{
private:
	bool _bDiagonal;
public:

	funcUpStreamTotalLDD(bool bDiagonal):_bDiagonal(bDiagonal) {}
	
	inline double op(DblRasterMx::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		double dblStream = 0.0;

		for (unsigned char cc = 1; cc < 10; cc++){
			
			if (cc==5)
				continue;

			if ( isStreamFrom(iLDD,cc) ){
				
				if ( _bDiagonal && (cc==1 || cc==3 || cc==7 || cc==9 )) 
					dblStream+=iDepth.chain_code(cc)*SQRT2REC;
				else
					dblStream+=iDepth.chain_code(cc);
			}
		}

		return dblStream;
	}

	inline double safeop(DblRasterMx::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		double dblStream = 0.0;

		for (unsigned char cc = 1; cc < 10; cc++){
			
			if (cc==5 || !iLDD.isValidItemByChainCode(cc))
				continue;

			if ( isStreamFrom(iLDD,cc) ){
				
				if ( _bDiagonal && (cc==1 || cc==3 || cc==7 || cc==9 )) 
					dblStream+=iDepth.chain_code(cc)*SQRT2REC;
				else
					dblStream+=iDepth.chain_code(cc);
			}
		}

		return dblStream;
	}
};

class funcUpStreamTotalMLDD: public funcUpStreamBaseMLDD
{
protected:
	double _dblTmp;
	bool _bDiagonal;
	double _dblCurrentStream;
public:
	
	funcUpStreamTotalMLDD(bool bDiagonal,bool inFlow):funcUpStreamBaseMLDD(inFlow),_dblTmp(0.0),_bDiagonal(bDiagonal),_dblCurrentStream(0.0){}

	inline double op(MultiflowDMatrix::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		

		double dblStream = 0.0;
		
		for (unsigned char cc = 1; cc < 10; cc++){
			
			if (cc==5)
				continue;
				
			_dblTmp = getLDDFrom(iLDD,cc);
			
			if ( _dblTmp > DoubleUtil::sDeltaD6 ){
				
				if ( _bDiagonal && (cc==1 || cc==3 || cc==7 || cc==9 )) 
					_dblCurrentStream=iDepth.chain_code(cc)*SQRT2REC;
				else 
					_dblCurrentStream=iDepth.chain_code(cc);

				if (_inFlow)
					_dblCurrentStream*=_dblTmp;
				
				dblStream+=_dblCurrentStream;
			}
		}

		return dblStream;
	}

	inline double safeop(MultiflowDMatrix::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		double dblStream = 0.0;

		for (unsigned char cc = 1; cc < 10; cc++){
			
			if (cc==5 || !iLDD.isValidItemByChainCode(cc))
				continue;
				
			_dblTmp = getLDDFrom(iLDD,cc);

			if ( _dblTmp > DoubleUtil::sDeltaD6 ){
				
				if ( _bDiagonal && (cc==1 || cc==3 || cc==7 || cc==9 )) 
					_dblCurrentStream=iDepth.chain_code(cc)*SQRT2REC;
				else 
					_dblCurrentStream=iDepth.chain_code(cc);

				if (_inFlow)
					_dblCurrentStream*=_dblTmp;
				
				dblStream+=_dblCurrentStream;
			}
		}

		return dblStream;
	}
};

class funcUpStreamMaxLDD: public funcUpStreamBaseLDD
{

public:

	funcUpStreamMaxLDD(){}
	
	inline double op(DblRasterMx::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		double dblMax = 0.0;
		double dblStream = 0.0;
		bool bHasStream = false;
		
		iPos->set(iDepth.getRow(),iDepth.getCol());

		for (unsigned char cc = 1; cc < 10; cc++){
			
			if (cc==5)
				continue;

			if ( isStreamFrom(iLDD,cc) ){
				
				dblStream = iDepth.chain_code(cc);
				
				if (dblStream>dblMax){
					dblMax = dblStream;
					
					DblRasterMx::iterator neighbour;
					iDepth.neighbourIterator(cc,neighbour);
					iPos->set(neighbour.getRow(),neighbour.getCol());
				}

				bHasStream = true;
				
			}
		}	

		if (!bHasStream)
			return *iDepth;

		return dblMax;
	}

	inline double safeop(DblRasterMx::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		double dblStream = 0.0;
		double dblMax = 0.0;
		bool bHasStream = false;
		
		iPos->set(iDepth.getRow(),iDepth.getCol());

		for (unsigned char cc = 1; cc < 10; cc++){
			
			if (cc==5 || !iLDD.isValidItemByChainCode(cc))
				continue;

			if ( isStreamFrom(iLDD,cc) ){
				
				dblStream = iDepth.chain_code(cc);
				
				if (dblStream>dblMax){
					
					dblMax = dblStream;
					
					DblRasterMx::iterator neighbour;
					iDepth.neighbourIterator(cc,neighbour);
					iPos->set(neighbour.getRow(),neighbour.getCol());
				}

				bHasStream = true;
			}
		}
		if (!bHasStream)
			return *iDepth;

		return dblMax;
	}
};


class funcUpStreamMaxMLDD: public funcUpStreamBaseMLDD
{
protected:
	double _dblTmp;
public:
	funcUpStreamMaxMLDD(bool inFlow):funcUpStreamBaseMLDD(inFlow),_dblTmp(0.0){}

	inline double op(MultiflowDMatrix::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		
		iPos->set(iDepth.getRow(),iDepth.getCol());

		double dblStream = 0.0;
		double dblMax = 0.0;
		bool bHasStream = false;

		for (unsigned char cc = 1; cc < 10; cc++){
			
			if (cc==5)
				continue;
				
			_dblTmp = getLDDFrom(iLDD,cc);
			
			if ( _dblTmp > DoubleUtil::sDeltaD6 ){
				
				dblStream=iDepth.chain_code(cc);
	
				if (_inFlow)
					dblStream*=_dblTmp;

				if (dblStream>dblMax) {
					dblMax = dblStream;
				
					DblRasterMx::iterator neighbour;
					iDepth.neighbourIterator(cc,neighbour);
					iPos->set(neighbour.getRow(),neighbour.getCol());
				}
				
				bHasStream = true;
			}
		}
		
		if (!bHasStream)
			return *iDepth;

		return dblMax;
	}

	inline double safeop(MultiflowDMatrix::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		double dblStream = 0.0;
		double dblMax = 0.0;
		bool bHasStream = false;
		
		iPos->set(iDepth.getRow(),iDepth.getCol());

		for (unsigned char cc = 1; cc < 10; cc++){
			
			if (cc==5 || !iLDD.isValidItemByChainCode(cc))
				continue;
				
			_dblTmp = getLDDFrom(iLDD,cc);

			if ( _dblTmp > DoubleUtil::sDeltaD6 ){
				
				dblStream=iDepth.chain_code(cc);
	
				if (_inFlow)
					dblStream*=_dblTmp;

				if (dblStream>dblMax) {
					dblMax = dblStream;
				
					DblRasterMx::iterator neighbour;
					iDepth.neighbourIterator(cc,neighbour);
					iPos->set(neighbour.getRow(),neighbour.getCol());
				}

				bHasStream = true;
			}
		}
		
		if (!bHasStream)
			return *iDepth;

		return dblMax;
	}
};

class funcUpStreamMinLDD: public funcUpStreamBaseLDD
{

public:

	funcUpStreamMinLDD(){}
	
	inline double op(DblRasterMx::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		double dblMin = DoubleUtil::getMAXValue();
		double dblStream = 0.0;
		bool bHasStream = false;

		iPos->set(iDepth.getRow(),iDepth.getCol());

		for (unsigned char cc = 1; cc < 10; cc++){
			
			if (cc==5)
				continue;

			if ( isStreamFrom(iLDD,cc) ){
				
				dblStream = iDepth.chain_code(cc);	

				if (dblStream<dblMin){
					dblMin = dblStream;
				
					DblRasterMx::iterator neighbour;
					iDepth.neighbourIterator(cc,neighbour);
					iPos->set(neighbour.getRow(),neighbour.getCol());
				
				}
				bHasStream = true;
				
			}
		}	

		if (!bHasStream)
			return *iDepth;

		return dblMin;
	}

	inline double safeop(DblRasterMx::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		double dblStream = 0.0;
		double dblMin = DoubleUtil::getMAXValue();
		bool bHasStream = false;
		
		iPos->set(iDepth.getRow(),iDepth.getCol());

		for (unsigned char cc = 1; cc < 10; cc++){
			
			if (cc==5 || !iLDD.isValidItemByChainCode(cc))
				continue;

			if ( isStreamFrom(iLDD,cc) ){
				
				dblStream = iDepth.chain_code(cc);	

				if (dblStream<dblMin) {
					dblMin = dblStream;
				
					DblRasterMx::iterator neighbour;
					iDepth.neighbourIterator(cc,neighbour);
					iPos->set(neighbour.getRow(),neighbour.getCol());
				}
				
				bHasStream = true;
				
			}
		}
		if (!bHasStream)
			return *iDepth;

		return dblMin;
	}
};

class funcUpStreamMinMLDD: public funcUpStreamBaseMLDD
{
protected:
	double _dblTmp;
public:
	funcUpStreamMinMLDD(bool inFlow):funcUpStreamBaseMLDD(inFlow),_dblTmp(0.0){}

	inline double op(MultiflowDMatrix::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		

		double dblStream = 0.0;
		double dblMin = DoubleUtil::getMAXValue();
		bool bHasStream = false;
		
		iPos->set(iDepth.getRow(),iDepth.getCol());

		for (unsigned char cc = 1; cc < 10; cc++){
			
			if (cc==5)
				continue;
				
			_dblTmp = getLDDFrom(iLDD,cc);
			
			if ( _dblTmp > DoubleUtil::sDeltaD6 ){
				
				dblStream=iDepth.chain_code(cc);

				if(_inFlow)
					dblStream*=_dblTmp;

				if (dblStream<dblMin) {
					dblMin = dblStream;
				
					DblRasterMx::iterator neighbour;
					iDepth.neighbourIterator(cc,neighbour);
					iPos->set(neighbour.getRow(),neighbour.getCol());
				
				}

				bHasStream = true;
			}
		}
		
		if (!bHasStream)
			return *iDepth;

		return dblMin;
	}

	inline double safeop(MultiflowDMatrix::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		double dblStream = 0.0;
		double dblMin = DoubleUtil::getMAXValue();;
		bool bHasStream = false;
		
		iPos->set(iDepth.getRow(),iDepth.getCol());

		for (unsigned char cc = 1; cc < 10; cc++){
			
			if (cc==5 || !iLDD.isValidItemByChainCode(cc))
				continue;
				
			_dblTmp = getLDDFrom(iLDD,cc);

			if ( _dblTmp > DoubleUtil::sDeltaD6 ){
				
				dblStream=iDepth.chain_code(cc);

				if(_inFlow)
					dblStream*=_dblTmp;

				if (dblStream<dblMin) {
					dblMin = dblStream;
				
					DblRasterMx::iterator neighbour;
					iDepth.neighbourIterator(cc,neighbour);
					iPos->set(neighbour.getRow(),neighbour.getCol());
				
				}
				bHasStream = true;
			}
		}
		
		if (!bHasStream)
			return *iDepth;

		return dblMin;
	}
};
class funcDownStreamMinMLDD: public funcDownStreamBaseMLDD
{

public:
	funcDownStreamMinMLDD(bool inFlow) : funcDownStreamBaseMLDD(inFlow) {}

	inline double op(MultiflowDMatrix::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		double dblMin = 0.0;
		double dblTmp = 0.0;
		
		iPos->set(iDepth.getRow(),iDepth.getCol());

		bool bFirst = true;

		for ( unsigned char cc = 1; cc<10; cc++) {
			
			if (cc==5)
				continue;
			
			double lddVal = (*iLDD).getByChainCode(cc);
			
			if (lddVal > DoubleUtil::sDeltaD6){
				
				bool setPos = false;

				if (_inFlow) {
					
					if (bFirst){
						dblMin = lddVal;
						bFirst = false;
						setPos = true;
					}else {
						if (lddVal<dblMin){
							dblMin = lddVal;
							setPos = true;
						}
					}

				}else{
					if (bFirst){
						dblMin = iDepth.chain_code(cc);
						bFirst = false;
						setPos = true;
					}else{
						dblTmp = iDepth.chain_code(cc);

						if (dblTmp<dblMin){
							dblMin = dblTmp;
							setPos = true;
						}
					}
				}
				
				if (setPos) {
					DblRasterMx::iterator neighbour;
					iDepth.neighbourIterator(cc,neighbour);
					iPos->set(neighbour.getRow(),neighbour.getCol());
				}
			}
		
		}
		
		if (_inFlow)
			return (*iDepth)*dblMin;

		return dblMin;
	}

	inline double safeop(MultiflowDMatrix::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		return op(iLDD,iDepth,iPos);
	}
};

class funcDownStreamMaxMLDD: public funcDownStreamBaseMLDD
{

public:
	funcDownStreamMaxMLDD(bool inFlow) : funcDownStreamBaseMLDD(inFlow) {}

	inline double op(MultiflowDMatrix::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		double dblMax = 0.0;
		double dblTmp = 0.0;
		bool bFirst = true;
		
		iPos->set(iDepth.getRow(),iDepth.getCol());

		for ( unsigned char cc = 1; cc<10; cc++) {

			if (cc==5)
				continue;
			
			double lddVal = (*iLDD).getByChainCode(cc);

			if (lddVal>DoubleUtil::sDeltaD6){
				
				bool setPos = false;
				if (_inFlow){
					if (lddVal>dblMax){
						dblMax = lddVal;
						setPos = true;
					}
				}else {
					if (bFirst){
						dblMax = iDepth.chain_code(cc);
						bFirst = false;
						setPos = true;
					}else{
						dblTmp = iDepth.chain_code(cc);

						if (dblTmp>dblMax){
							dblMax = dblTmp;
							setPos = true;
						}
					}
				}

				if (setPos) {
					DblRasterMx::iterator neighbour;
					iDepth.neighbourIterator(cc,neighbour);
					iPos->set(neighbour.getRow(),neighbour.getCol());
				}
			}
		
		}
		
		if (_inFlow)
			return (*iDepth)*dblMax;

		return dblMax;
	}

	inline double safeop(MultiflowDMatrix::iterator & iLDD, DblRasterMx::iterator & iDepth,RasterPositionMatrix::iterator & iPos)
	{
		return op(iLDD,iDepth,iPos);
	}
};
}

#endif