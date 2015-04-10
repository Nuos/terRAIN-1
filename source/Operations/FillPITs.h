#ifndef FILLPITS_H
#define FILLPITS_H

#include <boost/thread/thread.hpp>

#include <utility>
#include <list>

#include "UnaryMatrixFunc.h"
#include "Region.h"
#include "rastermatrix.h"
#include "paralellisable.h"



namespace TR
{

class FillPITFunc : public paralellisable
{
private:
	DblRasterMx::iterator & _iBegin;
	DblRasterMx::iterator & _iEnd;
	unsigned int _nPITs;
public:
	FillPITFunc(DblRasterMx::iterator & iBegin, DblRasterMx::iterator & iEnd)
		:_iBegin(iBegin), _iEnd(iEnd),_nPITs(0) {}

	unsigned int getPITNr() const
	{
		return _nPITs;
	}

	void run();
	virtual paralellisable * clone() const;
};

class FillPITs : public UnaryMatrixFunc
{
//private:
public:
	typedef std::pair<DblRasterMx::iterator,DblRasterMx::iterator> typeRegion;
	typedef std::list<typeRegion> typeRegionList;
private:
	unsigned int _nParts;
public:

	FillPITs(DblRasterMx & op, DblRasterMx & ret, unsigned int nParts);
	virtual void run();
	virtual paralellisable * clone() const;

//private:
public:
	
	void createRegions(DblRasterMx & matrix,typeRegionList & regions, typeRegionList & criticalRegions);
};

}

#endif
