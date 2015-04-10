#ifndef ACCFUNCTIONS_H
#define ACCFUNCTIONS_H

#include "rastermatrix.h"
#include "MultiflowDMatrix.h"

using namespace TR;

namespace FUNC
{

class funcAccFluxBase
{
protected:
	DblRasterMx & _mxDepth;
	DblRasterMx & _mxRet;
	bool _bLoopCheck;	
	double _dblFluxLimitRate;

public:
	
	funcAccFluxBase( DblRasterMx & mxDepth, DblRasterMx & mxRet,double dblFluxLimitRate ) : _mxDepth(mxDepth), _mxRet(mxRet),_bLoopCheck(true),_dblFluxLimitRate(dblFluxLimitRate)
	{
	}


};

class funcAccFluxOnLDD : public funcAccFluxBase
{

protected:
	size_t _nMaxIter;	
public:
	funcAccFluxOnLDD( DblRasterMx & mxDepth, DblRasterMx & mxRet,double dblFluxLimitRate ) 
		: funcAccFluxBase(mxDepth, mxRet,dblFluxLimitRate)
	{
		_nMaxIter = mxDepth.getColNr()*mxDepth.getRowNr();
	}

	//bool isRidgePoint( DblRasterMx::iterator & iIter ) const;
	void processPoint( DblRasterMx::iterator & iIter );

	void recursiveAccFluxCall( DblRasterMx::iterator & iLDD, double dblVal);
	void nonRecursiveAccFluxCall( DblRasterMx::iterator & iLDD);
};

class funcLoopCheck : public funcAccFluxBase
{

protected:
		size_t _nMaxIter;	
public:
	funcLoopCheck( DblRasterMx & mxDepth, DblRasterMx & mxRet,double dblFluxLimitRate ) 
		: funcAccFluxBase(mxDepth, mxRet,dblFluxLimitRate),_nMaxIter(0)
	{
		_nMaxIter = mxRet.getColNr()*mxRet.getRowNr();
	}

	//bool isRidgePoint( DblRasterMx::iterator & iIter ) const;
	void processPoint( DblRasterMx::iterator & iIter );
	bool call( DblRasterMx::iterator & iLDD);
};



class funcAccFluxOnMLDD : public funcAccFluxBase
{
	
public:
	funcAccFluxOnMLDD( DblRasterMx & mxDepth, DblRasterMx & mxRet,double dblFluxLimitRate ) 
		: funcAccFluxBase(mxDepth, mxRet,dblFluxLimitRate)
	{

	}

	//bool isRidgePoint( MultiflowDMatrix::iterator & iIter ) const;
	void processPoint( MultiflowDMatrix::iterator & iIter );
	
};

}
#endif