#ifndef MULTIFLOWACCFLUXONDRAINGRAPH_H
#define MULTIFLOWACCFLUXONDRAINGRAPH_H

#include "MultiflowDMatrix.h"


namespace TR {
	
class MultiflowAccfluxOnDrainGraph
{
private:

	class AccFluxPixelData
	{
	private:
		double _dblValue;
		bool   _bFinished;
	public:
		AccFluxPixelData():_dblValue(0.0),_bFinished(false){}
		bool isFinished() const 
		{ 
			return _bFinished;
		}

		double getValue() const 
		{ 
			return _dblValue;
		}

		void setValue(double dblValue) 
		{ 
			_dblValue = dblValue;
			_bFinished = true;
		}
	};

private:
	MultiflowDMatrix & _mfLDD;
	DblRasterMx & _mxDepth;
	DblRasterMx & _accflux;
	matrix<AccFluxPixelData> _accFluxData;
	double _dblFluxLimitRate;
public:
	MultiflowAccfluxOnDrainGraph(MultiflowDMatrix & mfLDD, DblRasterMx & mxDepth,DblRasterMx & retAccflux,double dblFluxLimitRate);

	void run();
	void processRidgePoint( MultiflowDMatrix::iterator & item);

};

}


#endif
