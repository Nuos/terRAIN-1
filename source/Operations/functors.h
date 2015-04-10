#ifndef FUNCTORS_H
#define FUNCTORS_H


#include "utils.h"
#include "DoubleUtil.h"
#include <cstdlib>
#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include "WindowIterator.h"
#include "rastermatrix.h"
#include "ModelParams.h"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>


using namespace TR;

namespace FUNC
{


class funcUniform
{

private:
	boost::minstd_rand generator;
	boost::uniform_real<> uni_dist;
    boost::variate_generator<boost::minstd_rand&, boost::uniform_real<> > uni;
public:
	
	funcUniform() : generator(static_cast<boost::int32_t>(time(NULL))),uni_dist(0,1), uni(generator, uni_dist){}
	
	inline void operator()(double & dblOp)
	{
		dblOp = uni();
		//srand( (unsigned)time( NULL ) );
		//dblOp = ((double)rand())/RAND_MAX ;
	}

};

class funcSum
{
private:
	double _dblSum;
public:
	funcSum():_dblSum(0.0)
	{}

	inline void operator()( double & dblOp )
	{
		_dblSum+=dblOp;
	}
	
	double getSum() const
	{
		return _dblSum;
	}

};
class funcAbs
{
public:
	inline double operator()( double & dblOp )
	{
		return std::fabs(dblOp);
	}
};

class funcAdd
{
public:
	inline double operator()( double  dblOp1, double dblOp2)
	{
		return dblOp1 + dblOp2;
	}
};

class funcSubt
{
public:
	inline double operator()( double  dblOp1, double dblOp2)
	{
		return dblOp1 - dblOp2;
	}
};

class funcMul
{
public:
	inline double operator()( double  dblOp1, double dblOp2)
	{
		return dblOp1 * dblOp2;
	}
};

class funcMax
{
public:
	inline double operator()( double  & dblOp1, double & dblOp2)
	{
		return std::max(dblOp1,dblOp2);
	}
};

class funcMin
{
public:
	inline double operator()( double  & dblOp1, double & dblOp2)
	{
		return std::min(dblOp1,dblOp2);
	}
};




class funcFraction
{
public:
	inline double operator()( double & dblOp1, double dblOp2)
	{
		return TR::DoubleUtil::cmpD6(dblOp2,0.0) ? TR::DoubleUtil::getNAN() : dblOp1/dblOp2;
	}
};


class funcPower
{
public:
	inline double operator()( double & dblOp1, double dblOp2)
	{
		return TR::DoubleUtil::cmpD6(dblOp1,0.0) &&  dblOp2< 0.0 ? TR::DoubleUtil::getNAN() : pow(dblOp1,dblOp2);
	}
};

class funcSin
{
private:
	bool _bRad;
public:
	funcSin(bool bRad):_bRad(bRad){}

	inline double operator()( double & dblOp )
	{
		return std::sin( _bRad ? dblOp : dblOp * DegToRad);
	}

};

class funcCos
{
private:
	bool _bRad;
public:
	funcCos(bool bRad):_bRad(bRad){}

	inline double operator()( double & dblOp )
	{
		return std::cos( _bRad ? dblOp : dblOp * DegToRad);
	}

};

class funcTan
{
private:
	bool	_bRad;
	double	_dblRad;
public:
	funcTan(bool bRad):_bRad(bRad), _dblRad(0.0){}

	inline double operator()( double & dblOp )
	{
		_dblRad = normalizeRad(_bRad ? dblOp : dblOp * DegToRad);
		
		return DoubleUtil::cmpD6(_dblRad,M_PI/2) || DoubleUtil::cmpD6(_dblRad,3*M_PI/2) ? DoubleUtil::getNAN() : std::tan(_dblRad);
	}

};

class funcCTan
{
private:
	bool	_bRad;
	double	_dblRad;
	double  _dblCos;
public:
	funcCTan(bool bRad):_bRad(bRad), _dblRad(0.0),_dblCos(0.0){}

	inline double operator()( double & dblOp )
	{
		_dblRad = normalizeRad(_bRad ? dblOp : dblOp * DegToRad);
		
		_dblCos = std::cos( _dblRad );

		if (DoubleUtil::cmpD6(_dblCos,0.0))
			return DoubleUtil::getNAN();

		return std::sin(_dblRad/_dblCos);
	}

};

class funcArSin
{
private:
	bool _bRad;
public:
	funcArSin(bool bRad):_bRad(bRad){}

	inline double operator()( double & dblOp )
	{
		if (dblOp<-1.0 || dblOp>1.0)
			return DoubleUtil::getNAN();

		return _bRad ? std::asin(dblOp) : std::asin(dblOp) * RadToDeg; 
	}

};

class funcArCos
{
private:
	bool _bRad;
public:
	funcArCos(bool bRad):_bRad(bRad){}

	inline double operator()( double & dblOp )
	{
		if (dblOp<-1.0 || dblOp>1.0)
			return DoubleUtil::getNAN();

		return _bRad ? std::acos(dblOp) : std::acos(dblOp) * RadToDeg; 
	}

};

class funcArTan
{
private:
	bool _bRad;
public:
	funcArTan(bool bRad):_bRad(bRad){}

	inline double operator()( double & dblOp )
	{

		return _bRad ? std::atan(dblOp) : std::atan(dblOp) * RadToDeg; 
	}

};

class funcArCTan
{
private:
	bool _bRad;
public:
	funcArCTan(bool bRad):_bRad(bRad){}

	inline double operator()( double & dblOp )
	{

		return _bRad ? -std::atan(dblOp) + M_PI/2 : (-std::atan(dblOp) + M_PI/2)* RadToDeg; 
	}

};

class funcLn
{
public:

	inline double operator()(double & dblOp)
	{
		return dblOp<=0.0 ? DoubleUtil::getNAN() : std::log(dblOp);
	}
};

class funcLog10
{
public:

	inline double operator()(double & dblOp)
	{
		return dblOp<=0.0 ? DoubleUtil::getNAN() : std::log10(dblOp);
	}
};

class funcExp
{
private:
	double _dblBase;
public:
	
	funcExp(double dblBase) : _dblBase(dblBase) {}

	inline double operator()(double & dblOp)
	{
		return std::pow(_dblBase,dblOp);
	}

};




class funcCoverWithMatrix
{
public:
	inline double operator()( double & dblOp1, double dblOp2)
	{
		return DoubleUtil::isNAN(dblOp1) ? dblOp2 : dblOp1;
	}


};

class funcCoverWithScalar
{

private:
	double _dblProxyVal;
public:
	
	funcCoverWithScalar( double dblProxy ) : _dblProxyVal(dblProxy) {}

	inline double operator()(double & dblOp)
	{
		return DoubleUtil::isNAN(dblOp) ? _dblProxyVal : dblOp;
	}
};

class MinCardinalAndDiagonal
{
private:
	double			_dblMinCardVal;
	unsigned char	_minCardChainCode;
	double			_dblMinDiagVal;
	unsigned char	_minDiagChainCode;

public:
	
	MinCardinalAndDiagonal() : _dblMinCardVal(0.0), _dblMinDiagVal(0.0) 
	{
		reset();
	}
	
	inline void reset()
	{
		_minCardChainCode = 0;
		_minDiagChainCode = 0;
	}

	inline void store(unsigned char nChainCode, double dblVal)
	{
		// cardinal chain code directions:
		if ( nChainCode==2 || nChainCode==4 || nChainCode==6 || nChainCode==8 ) {
			
			if ( _minCardChainCode==0 || dblVal<_dblMinCardVal){
				_dblMinCardVal	  = dblVal;
				_minCardChainCode = nChainCode;
			}
		
		}else { // diagonal chain code dierctions:
			
			if ( _minDiagChainCode==0 || dblVal<_dblMinDiagVal){
				_dblMinDiagVal	  = dblVal;
				_minDiagChainCode = nChainCode;
			}
		}
	}

	inline double getMinCardVal() const
	{
		return _dblMinCardVal;
	}

	inline unsigned char getMinCardChainCode() const
	{
		return _minCardChainCode;
	}

	inline double getMinDiagVal() const
	{
		return _dblMinDiagVal;
	}

	inline unsigned char getMinDiagChainCode() const
	{
		return _minDiagChainCode;
	}


};

/*
class funcSlope
{

private:

	double _dblMinVal;
	int	   _minRow;
	int	   _minCol;
	
	inline void init()
	{
		_minRow = -1000;
		_minCol = -1000;
	}
	
	inline void store( int nRow, int nCol, double dblVal )
	{
		
		if ( (_minRow ==-1000 && _minCol==-1000) || dblVal < _dblMinVal || (DoubleUtil::cmpD6(dblVal,_dblMinVal) && (nRow==0 || nCol==0))) {
			_minRow		= nRow;
			_minCol		= nCol;
			_dblMinVal  = dblVal;
		}
	}

	inline void computeSlope(DblRasterMx::iterator & iRet, double dblVal) 
	{
		DblRasterMx & owner = iRet.getOwner();

		double dx =  _minRow==0 || _minCol==0  ? owner.getPixelSize() : owner.getPixelSize() * SQRT2;
		
		*iRet = (dblVal<_dblMinVal && iRet.isOnBound()) ? 0.0 : (dblVal-_dblMinVal)/dx;
	}

public:
	
	funcSlope():_dblMinVal(0.0)
	{
		init();
	}
	
	
	inline void op(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		
	
		init();
		
		store(-1,-1,iOp(-1,-1));
		store(-1, 0,iOp(-1, 0));
		store(-1, 1,iOp(-1, 1));
		store( 0,-1,iOp(0,-1));
		store( 0, 1,iOp(0, 1));
		store( 1,-1,iOp(1,-1));
		store( 1, 0,iOp(1, 0));
		store( 1, 1,iOp(1, 1));

		computeSlope(iRet,*iOp);
		
		
	}

	inline void safeop(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		
		
		init();
		
		double dblTmp = 0.0;
		
		int i = -1;
		int j = -1;
		
		for (i = -1 ; i < 2; i++ ){
			for (j = -1 ; j < 2; j++ ) {
				
				if ( !(i==0 && j==0) && iOp.getItemSafe(i,j,dblTmp))
					store(i,j,dblTmp);
			}
		}
		
		computeSlope(iRet,*iOp);

		
	}

	 

 

};

*/


class funcSlope
{

private:

	MinCardinalAndDiagonal _minCardAndDiag;


	inline void computeSlope(DblRasterMx::iterator & iRet, double dblVal) 
	{		
		double dblTmp		= iRet.getOwner().getPixelSize();
		double dblCardSlope = (dblVal - _minCardAndDiag.getMinCardVal())/dblTmp;
		double dblDiagSlope = (dblVal - _minCardAndDiag.getMinDiagVal())/(dblTmp*SQRT2);
		
		dblTmp = dblDiagSlope > dblCardSlope ? dblDiagSlope : dblCardSlope;  
		
		
		*iRet = (dblTmp<0.0 && iRet.isOnBound()) ? 0.0 : dblTmp;
	}

public:
	
	funcSlope()
	{}
	
	
	inline void op(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		_minCardAndDiag.reset();
		
		for (unsigned char i = 1; i < 10; i++){
			if (i!=5)
				_minCardAndDiag.store(i,iOp.chain_code(i));
		}

		computeSlope(iRet,*iOp);
	}

	inline void safeop(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{	
		_minCardAndDiag.reset();
		
		double dblTmp = 0.0;

		for (unsigned char i = 1; i < 10; i++){
			if (i!=5 && iOp.chain_code_safe(i,dblTmp))
				_minCardAndDiag.store(i,dblTmp);
		}
		
		computeSlope(iRet,*iOp);
	}

	 

 

};

class funcPIT
{

private:

	MinCardinalAndDiagonal _minCardAndDiag;


	inline void computeSlope(DblRasterMx::iterator & iRet, double dblVal) 
	{		
		double dblTmp		= iRet.getOwner().getPixelSize();
		double dblCardSlope = (dblVal - _minCardAndDiag.getMinCardVal())/dblTmp;
		double dblDiagSlope = (dblVal - _minCardAndDiag.getMinDiagVal())/(dblTmp*SQRT2);
		
		dblTmp = dblDiagSlope > dblCardSlope ? dblDiagSlope : dblCardSlope;  
		
		if (dblTmp<0.0 && !iRet.isInOutflowPos())
			*iRet = 1.0;
		else
			*iRet = 0.0;
		
	}

public:
	
	funcPIT()
	{}
	
	
	inline void op(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		_minCardAndDiag.reset();
		
		for (unsigned char i = 1; i < 10; i++){
			if (i!=5)
				_minCardAndDiag.store(i,iOp.chain_code(i));
		}

		computeSlope(iRet,*iOp);
	}

	inline void safeop(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{	
		_minCardAndDiag.reset();
		
		double dblTmp = 0.0;

		for (unsigned char i = 1; i < 10; i++){
			if (i!=5 && iOp.chain_code_safe(i,dblTmp))
				_minCardAndDiag.store(i,dblTmp);
		}
		
		computeSlope(iRet,*iOp);
	}

	 

 

};


/*
// local drain direction
class funcLDD
{

private:
	unsigned char  _nLDD;
	double		   _dblMinVal;
	
	void inline init()
	{
		_nLDD		= 0;
		_dblMinVal	= 0.0;
	}
	
	inline void store(double dblVal, unsigned char nChainCode)
	{
		if (_nLDD==0 || dblVal<_dblMinVal) {
			_nLDD		= nChainCode;
			_dblMinVal	= dblVal;	
		}
	}

	inline double computeLDD(const DblRasterMx::iterator & iOp) 
	{
		if ( _dblMinVal < *iOp || !iOp.isInOutflowPos())
			return _nLDD;
		else 
			return 5;	
	}
public:
	
	funcLDD()
	{
		init();
	}
	
	
	inline void op(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		init();

		for (unsigned char i = 1; i < 10; i++){
			if (i!=5)
				store(iOp.chain_code(i),i);
		}
		
		*iRet = computeLDD(iOp);
	}
	
	inline void safeop(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		init();

		double dblTmp = 0.0;

		for (unsigned char i = 1; i < 10; i++){
			if (i!=5 && iOp.chain_code_safe(i,dblTmp))
				store(dblTmp,i);
		}

		*iRet = computeLDD(iOp);
	
	}
};
*/

class funcLDD
{

private:
	MinCardinalAndDiagonal _minCardAndDiag;
	
	inline double computeLDD(const DblRasterMx::iterator & iOp) 
	{
		double dblMinCardVal = _minCardAndDiag.getMinCardVal();
		double dblMinDiagVal = _minCardAndDiag.getMinDiagVal();
		
		double dblCurrentVal = *iOp;

		if ( (dblMinCardVal<dblCurrentVal || dblMinDiagVal<dblCurrentVal) || !iOp.isInOutflowPos()){
			
			if ((dblMinCardVal-dblMinDiagVal)>(dblCurrentVal-dblMinDiagVal)*CARDDIAGDELTA)
				return _minCardAndDiag.getMinDiagChainCode();
			else
				return _minCardAndDiag.getMinCardChainCode();	

		}else
			return 5;
		
	}
public:
	
	funcLDD()
	{}
	
	
	inline void op(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		_minCardAndDiag.reset();

		for (unsigned char i = 1; i < 10; i++){
			if (i!=5)
				_minCardAndDiag.store(i,iOp.chain_code(i));
		}
		
		*iRet = computeLDD(iOp);
	}
	
	inline void safeop(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		_minCardAndDiag.reset();

		double dblTmp = 0.0;

		for (unsigned char i = 1; i < 10; i++){
			if (i!=5 && iOp.chain_code_safe(i,dblTmp))
				_minCardAndDiag.store(i,dblTmp);
		}

		*iRet = computeLDD(iOp);
	}
};


/*
class funcFillPit
{
public:

	funcFillPit()
	{
	}
	
	inline void op(const DblRasterMx::iterator & iLDD, const DblRasterMx::iterator & iRasters, DblRasterMx::iterator & iRet)
	{
		*iRet=*iRasters;
	}

	inline void safeop(const DblRasterMx::iterator & iLDD, const DblRasterMx::iterator & iRasters, DblRasterMx::iterator & iRet)
	{
		*iRet=*iRasters;
	}


};
*/

class funcDiagonal
{
private:
	double sumDiagonal;
	double sumCardinal;


public:
	
	funcDiagonal():sumDiagonal(0.0),sumCardinal(0.0){}

	inline double operator()(MultiflowDMatrix::iterator & item)
	{
		
		DoubleChainCodeData & chainCodeData = *item;
		
		double sumDiagonal = chainCodeData.getByChainCode(1) + 
						     chainCodeData.getByChainCode(3) +
							 chainCodeData.getByChainCode(7) +
							 chainCodeData.getByChainCode(9);

		double sumCardinal = chainCodeData.getByChainCode(2) +
							 chainCodeData.getByChainCode(4) +
							 chainCodeData.getByChainCode(6) +
							 chainCodeData.getByChainCode(8);

		return sumCardinal + sumDiagonal*SQRT2REC;
	
	}
};

class funcSignStream
{
private:
	double _mul;
public:
	
	funcSignStream(double mul):_mul(mul) 
	{}
	
	
	inline void op(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{
		safeop(iOp,iRet );
	}

	inline void safeop(const DblRasterMx::iterator & iOp, DblRasterMx::iterator & iRet)
	{	
		double currentVal = *iOp;
		double dblTmp = 0.0;
		
		unsigned char i = 0;

		// megkeressük az elsõ olyan szomszédot, mely alacsonyabban van
		for ( i = 1; i < 10; i++){
			if (i!=5 && iOp.chain_code_safe(i,dblTmp)){
				if ( dblTmp < currentVal)
					break;	
			}	
		}
		
		// nincs alacsonyabban lévõ pixel, vagyis az aktuális pixel egy PIT
		if (i==10){
			*iRet = -1;
			return;
		}
		
		unsigned char n = i+1;

		// megkeressük az elsõ olyan szomszédot, mely magasabban van, mint az aktulis pixel
		for ( i = 0; i < 7; i++, n++){
			
			if (n==10)
				n=1;

			if (n!=5 && iOp.chain_code_safe(n,dblTmp)){
				if ( dblTmp > currentVal)
					break;	
			}	
		}	
		
		// nincs magasabban lévõ pixel, az aktuális pixel egy lokális maximum
		if (i==10){
			*iRet = 0;
			return;
		}

		// csak akkor lesz része a folyamnak a pixel, 
		// ha a magasabban lévõ szomszédok egy összefüggõ régiót alkotnak

		double values[10];
		values[0] = currentVal;
		values[1] = dblTmp;
		int nVal = 2;
		int nDown = 0;

		n++;
		for (i = 0; i < 7; i++, n++){
			
			if (n==10)
				n=1;

			if (n!=5 && iOp.chain_code_safe(n,dblTmp)){
				
				if ( dblTmp > currentVal) {
					
					// nem összefüggõ a régió
					if (nDown>0){
						*iRet = 0;
						return;	
					}
					values[nVal] = dblTmp;
					++nVal;
				}else
					nDown++;
			}	
		}

		// legalább 3 olyan szomszéd kell, mely magasabban van
		if (nVal<4){
			*iRet = 0;
			return;	
		}
		
		values[nVal] = currentVal;

		int local_max_places[8];
		int nMax = 0;
		
		// a maximum helyek közül a legkisebb
		double dblMaxInf = DoubleUtil::getMAXValue();
		
		for (i = 1; i < nVal; i++ ){
			dblTmp = values[i];
			if (values[i-1]<dblTmp && dblTmp>values[i+1]){
				local_max_places[nMax]=i;
				nMax++;
				i++;

				if (dblTmp<dblMaxInf)
					dblMaxInf = dblTmp;
			}
		}
		
		// legalább 2 lokális maximum hely kell
		if (nMax<2){
			*iRet = 0;
			return;
		}
		
		// maximum helyek közti lokális minimumok maximuma
		double dblMinSup = -DoubleUtil::getMAXValue();
 
		int b = 0, e = 0, j = 0;
		
		for (i = 0; i<nMax-1; i++){
			
			b = local_max_places[i];
			e = local_max_places[i+1];
			dblTmp = values[b];
			b++;
			
			for ( j = b; j < e; j++){
				if (values[j]<dblTmp){
					dblTmp = values[j];	
				}
			}

			if (dblTmp>dblMinSup){
				dblMinSup = dblTmp;	
			}
		}
		
		if (fabs(dblMinSup)<fabs(dblMaxInf)*_mul){
			*iRet = 1;	
			return;
		}

		*iRet = 0;
	}
};

class funcFindStreamMLDD
{
private:
	double _mul;
public:
	
	funcFindStreamMLDD(double mul):_mul(mul) 
	{}

	inline double operator()(MultiflowDMatrix::iterator & iOp)
	{
		unsigned char i = 0;
		
		double inDrainValues[8];
		
		DoubleChainCodeData neighbour;
		int n = 0;
		int zeroCounter = 0;
		double tmp = 0.0;

		for ( i = 1; i < 10; i++){
			
			if (i==5)
				continue;

			if (iOp.chain_code_safe(i,neighbour)){
				tmp = neighbour.getByChainCode(myChainCode(i));
				inDrainValues[n] = tmp < DoubleUtil::sDeltaD6 ? 0.0 : tmp;
			}else
				inDrainValues[n] = 0.0;

			if (DoubleUtil::cmpD6(inDrainValues[n],0.0))
				zeroCounter++;
			
			n++;
			
		}
		
		//ez egy PIT
		if (zeroCounter==0)
			return -1;
		
		// megszámoljuk hány olyan hely van a tömbben ahol 0 és nem 0 számok állnak egymás mellett
		// ha ez a szám nagyobb mint 2, akkor a befolyó pixelek nem egy "legyezõt" alkotnak
		int boundCounter = 0;
		
		// pozitív, szigorú lokális maximumhelyek száma
		// (ha van két lokális maximum hely, akkor szükségképp lennnie kell egy lokális minimum helynek is köztük)
		int nMax = 0;
		
		// lokális minimum helyek közt a legnagyobb
		double minSup = 0.0;
		// lokális maximum helyek közt a legkisebb
		double maxInf = DoubleUtil::getMAXValue();
		
		double nextVal = 0.0;
		double prevVal = 0.0;
		double currentVal = 0.0;

		for ( n = 0; n < 8; n++ ){
			nextVal = inDrainValues[n==7 ? 0 : n+1]; 
			prevVal = inDrainValues[n==0 ? 7 : n-1];
			currentVal = inDrainValues[n];

			if ((DoubleUtil::cmpD6(currentVal,0.0) && nextVal>0) ||
				(DoubleUtil::cmpD6(nextVal,0.0) && currentVal>0) ) {
				boundCounter++;	
			}

			if (boundCounter>2)
				return 0.0;
			
			if (currentVal<DoubleUtil::sDeltaD6)
				continue;
			
			// lokális maximum hely
			if (currentVal>prevVal && currentVal>nextVal){
				nMax++;

				if (currentVal<maxInf)
					maxInf = currentVal;
				continue;
			}
			
			// lokális mimimumhely a "legyezõben"
			if ( (DoubleUtil::cmpD6(prevVal,0.0) || prevVal>currentVal ) &&
				 (DoubleUtil::cmpD6(nextVal,0.0) || nextVal>currentVal )){
				
				 if ( currentVal > minSup)
					 minSup = currentVal;
			}
		}
		
		if (nMax>1 && minSup < maxInf*_mul){
			return 1.0;
		}

		return 0.0;
	}
};

}

#endif