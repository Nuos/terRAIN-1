#include "OperationInterface.h"
#include "Paralellize.h"
#include "Coordinates.h"
#include "ModelParams.h"
#include "MultiflowLDD.h"
#include "accfluxdefs.h"
#include "StreamFuncDefs.h"
#include "longestFlowPathLengthDefs.h"
#include "FillPITs.h"
#include "MultiflowAccflux.h"
#include "MultiflowAccfluxOnDrainGraph.h"
#include <math.h>
#include <limits>


#ifdef VISUALIZATION
#include "RasterPolyDataFactory.h"
#include "PolyDataVisualization.h"
#include "DefaultInteractorFactory.h"
#include "RendererFactory.h"
#include "SimpleRenderWindowFactory.h"

using namespace VISUALIZATION;
#endif

#include "InterpolateRasterToSquares.h"
#include "MarkTouchedrasters.h"
#include "MLDDFunctions.h"
#include "SpreadLDD.h"
#include "SpreadLDDMax.h"
#include <map>


namespace TR
{

const unsigned int nThreadNr = 2;

#ifdef PARALELL_MODUL

#define _RUN( obj ) \
	Paralellize paralellize(obj,nThreadNr);\

#else

#define _RUN( obj ) \
	obj.run();\

#endif

void mapattr(size_t nRows, size_t nCols, double dblPixelSize,double dblInitValue, DblRasterMx & mxInit)
{
	mxInit.init(nRows,nCols,dblPixelSize,origoBottomLeft, dblInitValue);
}

void uniform( DblRasterMx & mxRet )
{
	mfUniform Obj(mxRet,mfUniform::typeFunctor());

	_RUN(Obj)
}

void add(  DblRasterMx & mxOp1, DblRasterMx & mxOp2, DblRasterMx & mxRet )
{
	mfAdd addObj(mxOp1,mxOp2,mxRet,mfAdd::typeFunctor());

	_RUN(addObj)
}

void sup( DblRasterMx & mxOp1, DblRasterMx & mxOp2, DblRasterMx & mxRet )
{
	mfSubt supObj(mxOp1,mxOp2,mxRet,mfSubt::typeFunctor());

	_RUN(supObj)
}

void mul( DblRasterMx & mxOp1, DblRasterMx & mxOp2, DblRasterMx & mxRet )
{
	mfMul mulObj(mxOp1,mxOp2,mxRet,mfMul::typeFunctor());

	_RUN(mulObj)
}

void fraction( DblRasterMx & mxOp1, DblRasterMx & mxOp2, DblRasterMx & mxRet )
{
	mfFraction fractionObj(mxOp1,mxOp2,mxRet,mfFraction::typeFunctor());

	_RUN(fractionObj)
}

void power( DblRasterMx & mxOp1, DblRasterMx & mxOp2, DblRasterMx & mxRet )
{
	mfPower powerObj(mxOp1,mxOp2,mxRet,mfPower::typeFunctor());

	_RUN(powerObj)
}

void abs( DblRasterMx & mxOp, DblRasterMx & mxRet )
{
	mfAbs absObj(mxOp,mxRet,mfAbs::typeFunctor());

	_RUN(absObj)
}

void sin(  DblRasterMx & mxOp, bool bRad, DblRasterMx & mxRet)
{
	mfSin sinObj(mxOp,mxRet,mfSin::typeFunctor(bRad));

	_RUN(sinObj)

}

void cos(  DblRasterMx & mxOp, bool bRad,  DblRasterMx & mxRet)
{
	mfCos cosObj(mxOp,mxRet,mfCos::typeFunctor(bRad));

	_RUN(cosObj)

}

void tg(  DblRasterMx & mxOp, bool bRad,  DblRasterMx & mxRet)
{
	mfTan tanObj(mxOp,mxRet,mfTan::typeFunctor(bRad));

	_RUN(tanObj)

}

void ctg(  DblRasterMx & mxOp, bool bRad, DblRasterMx & mxRet)
{
	mfCTan ctanObj(mxOp,mxRet,mfCTan::typeFunctor(bRad));
	
	_RUN(ctanObj)

}

void asin(  DblRasterMx & mxOp, bool bRad, DblRasterMx & mxRet)
{
	mfArSin Obj(mxOp,mxRet,mfArSin::typeFunctor(bRad));
	
	_RUN(Obj)
}

void acos(  DblRasterMx & mxOp, bool bRad, DblRasterMx & mxRet)
{
	mfArCos Obj(mxOp,mxRet,mfArCos::typeFunctor(bRad));
	
	_RUN(Obj)
}

void atg(  DblRasterMx & mxOp, bool bRad, DblRasterMx & mxRet)
{
	mfArTan Obj(mxOp,mxRet,mfArTan::typeFunctor(bRad));
	
	_RUN(Obj)
}

void actg(  DblRasterMx & mxOp,bool bRad, DblRasterMx & mxRet)
{
	mfArCTan Obj(mxOp,mxRet,mfArCTan::typeFunctor(bRad));
	
	_RUN(Obj)
}


void ln(  DblRasterMx & mxOp, DblRasterMx & mxRet)
{
	mfLn Obj(mxOp,mxRet,mfLn::typeFunctor());

	_RUN(Obj)
}

void log10(  DblRasterMx & mxOp, DblRasterMx & mxRet)
{
	mfLog10 Obj(mxOp,mxRet,mfLog10::typeFunctor());

	_RUN(Obj)
}

void exp(  DblRasterMx & mxOp,double dblBase, DblRasterMx & mxRet)
{
	mfExp Obj(mxOp,mxRet,mfExp::typeFunctor(dblBase));

	_RUN(Obj)
}

void cover(  DblRasterMx & mxOp1,  DblRasterMx & mxOp2, DblRasterMx & mxRet )
{
	mfCovewWithMatrix Obj(mxOp1,mxOp2,mxRet,mfCovewWithMatrix::typeFunctor());

	_RUN(Obj)

}

void cover(  DblRasterMx & mxOp1, double dblProxy, DblRasterMx & mxRet )
{
	mfCovewWithScalar Obj(mxOp1,mxRet,mfCovewWithScalar::typeFunctor(dblProxy));

	_RUN(Obj)
}

void xcoordinate( DblRasterMx & mxOp, DblRasterMx & mxRet)
{
	Coordinates obj(ctX,mxOp,mxRet);

	obj.run();
}

void ycoordinate( DblRasterMx & mxOp, DblRasterMx & mxRet)
{
	Coordinates obj(ctY,mxOp,mxRet);

	obj.run();
}

void slope(  DblRasterMx & mxOp, DblRasterMx & mxRet )
{
	
	mxRet.initlike(mxOp);

	mfSlope Obj(mxOp.begin(),mxOp.end(),mxRet.begin(),1,mfSlope::typeFunctor());

	_RUN(Obj)
}

void lddcreate(  DblRasterMx & mxOp, DblRasterMx & mxRet,bool bFillPits )
{
	
	mxRet.initlike(mxOp);

	DblRasterMx::iterator iRet = mxRet.begin();

	if (bFillPits){
		DblRasterMx mx1;

#ifdef PARALELL_MODUL
		FillPITs obj(mxOp,mx1,1);
		//FillPITs obj(mxOp,mx1,nThreadNr);
#else	
		FillPITs obj(mxOp,mx1,1);
#endif
		obj.run();

		DblRasterMx::iterator iMx1 = mx1.begin(), endMx1 = mx1.end();
		mfLDD Obj(iMx1,endMx1,iRet,1,mfLDD::typeFunctor());

		_RUN(Obj)
	}else{

		DblRasterMx::iterator iOp = mxOp.begin(), endOp = mxOp.end();

		mfLDD Obj(iOp,endOp ,iRet ,1,mfLDD::typeFunctor());
		_RUN(Obj)
	}
}

void findpits(  DblRasterMx & mxOp, DblRasterMx & mxRet )
{
	mxRet.initlike(mxOp);
	
	DblRasterMx::iterator iOp = mxOp.begin(), endOp = mxOp.end();
	DblRasterMx::iterator iRet = mxRet.begin();
	mfFindPITs Obj(iOp ,endOp ,iRet ,1,mfFindPITs::typeFunctor());

	_RUN(Obj)
}

void setOutflowType(eOutflowType outflowType)
{
	ModelParams::getModelParamsObj().setOutflowType(outflowType);	
}
/*
void fillpits(  DblRasterMx & mxLDD,DblRasterMx & mxRasters, DblRasterMx & mxRet  )
{
	mxRet.initlike(mxRasters);	

	mfFillPits Obj( mxLDD.begin() , mxLDD.end(), mxRasters.begin(), mxRet.begin(),1,mfFillPits::typeFunctor());

	_RUN(Obj)
}
*/
void semiMultiflowLDD(DblRasterMx & mxOp, MultiflowDMatrix & mxRet, bool bFillPits)
{
	mxRet.initlike(mxOp);
	MultiflowDMatrix::iterator iRet = mxRet.begin();
	

	if (bFillPits){
		DblRasterMx mx1;

#ifdef PARALELL_MODUL
		//FillPITs obj(mxOp,mx1,nThreadNr);
		FillPITs obj(mxOp,mx1,1);
#else	
		FillPITs obj(mxOp,mx1,1);
#endif
		obj.run();

		DblRasterMx::iterator iOp = mx1.begin(), endOp = mx1.end();

		MultiflowLDD Obj(1.0,iOp,endOp, iRet ,bFillPits,true,false);

		_RUN(Obj)
	}else{
		DblRasterMx::iterator iOp = mxOp.begin(), endOp = mxOp.end();

		MultiflowLDD Obj(1.0,iOp, endOp,iRet,bFillPits,true, false);

		_RUN(Obj)
	}
}

void multiflowLDD(double a, DblRasterMx & mxOp, MultiflowDMatrix & mxRet, bool bFillPits)
{
	mxRet.initlike(mxOp);
	MultiflowDMatrix::iterator iRet = mxRet.begin();
	

	if (bFillPits){
		DblRasterMx mx1;

#ifdef PARALELL_MODUL
		//FillPITs obj(mxOp,mx1,nThreadNr);
		FillPITs obj(mxOp,mx1,1);
#else	
		FillPITs obj(mxOp,mx1,1);
#endif
		obj.run();

		DblRasterMx::iterator iOp = mx1.begin(), endOp = mx1.end();
		MultiflowLDD Obj(a, iOp,endOp, iRet,bFillPits,false, false);

		_RUN(Obj)
	}else{
		DblRasterMx::iterator iOp = mxOp.begin(), endOp = mxOp.end();
		MultiflowLDD Obj(a, iOp, endOp, iRet,bFillPits,false, false);

		_RUN(Obj)
	}

}

void multiflowLDD(DblRasterMx a, DblRasterMx & mxOp, MultiflowDMatrix & mxRet, bool bFillPits)
{
	mxRet.initlike(mxOp);
	MultiflowDMatrix::iterator iRet = mxRet.begin();
	

	if (bFillPits){
		DblRasterMx mx1;

#ifdef PARALELL_MODUL
		//FillPITs obj(mxOp,mx1,nThreadNr);
		FillPITs obj(mxOp,mx1,1);
#else	
		FillPITs obj(mxOp,mx1,1);
#endif
		obj.run();
		DblRasterMx::iterator iOp = mx1.begin(), endOp = mx1.end();
		MultiflowLDD Obj(iOp, endOp,iRet,bFillPits,false,a.begin(), false);

		_RUN(Obj)
	}else{
		DblRasterMx::iterator iOp = mxOp.begin(), endOp = mxOp.end();
		DblRasterMx::iterator iA = a.begin();
		MultiflowLDD Obj(iOp, endOp,iRet,bFillPits,false,iA, false);

		_RUN(Obj)
	}
}

void check_loop( DblRasterMx & mxLDD, DblRasterMx & mxRet)
{
	DblRasterMx mxDepth;
	
	mxRet.initlike(mxLDD);
	
	double dblTmp = 0.0;
	mxRet.fill(dblTmp);

	DblRasterMx::iterator iLDD = mxLDD.begin(), endLDD = mxLDD.end();
	loopCheckFuncOnLDD Obj(iLDD, endLDD ,mxDepth, mxRet );

	Obj.run();
}
void accflux( DblRasterMx & mxLDD, DblRasterMx & mxDepth, DblRasterMx & mxRet)
{
	mxRet.initlike(mxDepth);
	
	double dblTmp = 0.0;
	mxRet.fill(dblTmp);

	accfluxFuncOnLDD Obj(mxLDD.begin(), mxLDD.end(),mxDepth, mxRet );

	_RUN(Obj)
}

void accflux( MultiflowDMatrix & mxLDD, DblRasterMx & mxDepth, DblRasterMx & mxRet, double dblFluxLimitRate)
{
	MultiflowAccfluxOnDrainGraph obj(mxLDD,mxDepth,mxRet,dblFluxLimitRate);
	
	obj.run();
	/*
	mxRet = mxDepth;

	MultiflowAccflux Obj(mxLDD.begin(), mxLDD.end(),mxDepth, mxRet,dblFluxLimitRate );

	_RUN(Obj)
	*/
}

void upstreamtotal( DblRasterMx & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,bool bDiagonal)
{
	mxRet.initlike(mx);
	
	// this method do not use this really
	RasterPositionMatrix mxPosDummy;
	mxPosDummy.initlike(mx);

	mfLDDUpStreamTotal Obj(mxLDD.begin(),mxLDD.end(), mx.begin(),mxRet.begin(),mxPosDummy.begin(),mfLDDUpStreamTotal::typeFunctor(bDiagonal));

	_RUN(Obj)
}

void upstreamtotal( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,bool bDiagonal,bool inFlow)
{
	mxRet.initlike(mx);
	
	// this method do not use this really
	RasterPositionMatrix mxPosDummy;
	mxPosDummy.initlike(mx);

	mfMLDDUpStreamTotal Obj(mxLDD.begin(),mxLDD.end(), mx.begin(),mxRet.begin(),mxPosDummy.begin(),mfMLDDUpStreamTotal::typeFunctor(bDiagonal,inFlow));

	_RUN(Obj)
}

void upstreammax( DblRasterMx & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,RasterPositionMatrix & mxPos)
{
	mxRet.initlike(mx);
	mxPos.initlike(mx);

	mfLDDUpStreamMax Obj(mxLDD.begin(),mxLDD.end(), mx.begin(),mxRet.begin(),mxPos.begin(),mfLDDUpStreamMax::typeFunctor());

	_RUN(Obj)
}

void upstreammax( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,RasterPositionMatrix & mxPos, bool inFlow)
{
	mxRet.initlike(mx);
	mxPos.initlike(mx);

	mfMLDDUpStreamMax Obj(mxLDD.begin(),mxLDD.end(), mx.begin(),mxRet.begin(),mxPos.begin(),mfMLDDUpStreamMax::typeFunctor(inFlow));

	_RUN(Obj)
}


void upstreammin( DblRasterMx & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,RasterPositionMatrix & mxPos)
{
	mxRet.initlike(mx);
	mxPos.initlike(mx);

	mfLDDUpStreamMin Obj(mxLDD.begin(),mxLDD.end(), mx.begin(),mxRet.begin(),mxPos.begin(),mfLDDUpStreamMin::typeFunctor());

	_RUN(Obj)
}

void upstreammin( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,RasterPositionMatrix & mxPos,bool inFlow)
{

	mxRet.initlike(mx);
	mxPos.initlike(mx);

	mfMLDDUpStreamMin Obj(mxLDD.begin(),mxLDD.end(), mx.begin(),mxRet.begin(),mxPos.begin(),mfMLDDUpStreamMin::typeFunctor(inFlow));

	_RUN(Obj)
}


void downstreamtotal( DblRasterMx & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet)
{
	
	mxRet.initlike(mx);

	// this method do not use this really
	RasterPositionMatrix mxPosDummy;
	mxPosDummy.initlike(mx);

	mfLDDDownStreamTotal Obj(mxLDD.begin(),mxLDD.end(), mx.begin(),mxRet.begin(),mxPosDummy.begin(),mfLDDDownStreamTotal::typeFunctor());

	_RUN(Obj)
}

void downstreamtotal( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet)
{
	mxRet.initlike(mx);
	
	// this method do not use this really
	RasterPositionMatrix mxPosDummy;
	mxPosDummy.initlike(mx);

	mfMLDDDownStreamTotal Obj(mxLDD.begin(),mxLDD.end(), mx.begin(),mxRet.begin(),mxPosDummy.begin(),mfMLDDDownStreamTotal::typeFunctor());

	_RUN(Obj)
}

void longestflowpathlength(DblRasterMx & mxLDD, DblRasterMx & mxRet)
{
	mxRet.initlike(mxLDD);
	
	double dblTmp = 0.0;
	mxRet.fill(dblTmp);

	funcLongestFlowPathLengthLDD Obj( mxLDD.begin(), mxLDD.end(),mxRet);

	_RUN(Obj)
}

void longestflowpathlength(MultiflowDMatrix & mxLDD, DblRasterMx & mxRet)
{
	mxRet.initlike(mxLDD);
	
	double dblTmp = 0.0;
	mxRet.fill(dblTmp);

	funcLongestFlowPathLengthMLDD Obj( mxLDD.begin(), mxLDD.end(),mxRet);

	_RUN(Obj)
}

double mapavarage(DblRasterMx & mxOp)
{
	size_t n = mxOp.getColNr()*mxOp.getRowNr();
	
	if (n==0)
		return 0.0;

	mfSum::typeFunctor func;
	mfSum Obj(mxOp,func);

	Obj.run();

	return func.getSum()/n;
}

void setOutputDirectory(const char * lpszOutputDir)
{
	std::string strTmp(lpszOutputDir);
	
	TR::ModelParams::getModelParamsObj().setOutputDir(strTmp);
}


void downstreammin( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,RasterPositionMatrix & mxPos,bool inFlow)
{

	mxRet.initlike(mx);
	mxPos.initlike(mx);

	mfMLDDDownStreamMin Obj(mxLDD.begin(),mxLDD.end(), mx.begin(),mxRet.begin(),mxPos.begin(),mfMLDDDownStreamMin::typeFunctor(inFlow));

	_RUN(Obj)
}

void downstreammax( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,RasterPositionMatrix & mxPos, bool inFlow)
{
	mxRet.initlike(mx);
	mxPos.initlike(mx);

	mfMLDDDownStreamMax Obj(mxLDD.begin(),mxLDD.end(), mx.begin(),mxRet.begin(),mxPos.begin(),mfMLDDDownStreamMax::typeFunctor(inFlow));

	_RUN(Obj)
}

void min( DblRasterMx & mxOp1, DblRasterMx & mxOp2, DblRasterMx & mxRet )
{
	mfMin obj(mxOp1,mxOp2,mxRet,mfMin::typeFunctor());

	_RUN(obj)
}

void max( DblRasterMx & mxOp1, DblRasterMx & mxOp2, DblRasterMx & mxRet )
{
	mfMax obj(mxOp1,mxOp2,mxRet,mfMax::typeFunctor());

	_RUN(obj)
}

DblRasterMx operator+(DblRasterMx & op1, DblRasterMx & op2)
{
	
	DblRasterMx ret;

	add(op1,op2,ret);

	return ret;
}

DblRasterMx operator-(DblRasterMx & op1, DblRasterMx & op2)
{
	
	DblRasterMx ret;

	sup(op1,op2,ret);

	return ret;
}

DblRasterMx operator/(DblRasterMx & op1, DblRasterMx & op2)
{
	
	DblRasterMx ret;

	fraction(op1,op2,ret);

	return ret;
}

DblRasterMx operator*(DblRasterMx & op1, DblRasterMx & op2)
{
	
	DblRasterMx ret;

	mul(op1,op2,ret);

	return ret;
}

DblRasterMx operator^(DblRasterMx & op1, DblRasterMx & op2)
{
	
	DblRasterMx ret;

	power(op1,op2,ret);

	return ret;
}

void diagonal(MultiflowDMatrix & mxLDD,DblRasterMx & mxRet)
{	
	mxRet.initlike(mxLDD);
	
	mfDiagonal obj(mxLDD.begin(),mxLDD.end(),mxRet.begin(),mfDiagonal::typeFunctor());
	
	_RUN(obj);

}

void display(DblRasterMx & mx)
{
#ifdef VISUALIZATION
	RasterPolyDataFactory terrainToPolyData(mx);

	RenderWindowParams  renderWindowParams(400,600);
	SimpleRenderWindowFactory renderWindowFactory(renderWindowParams);

	DefaultInteractorFactory interactorFactory;
	
	RendererFactoryParams  rendererFactoryParams;
	RendererFactory rendererFactory(rendererFactoryParams);

	PolyDataVisualization visualization(rendererFactory,renderWindowFactory, interactorFactory,terrainToPolyData);

	visualization.visualize();
#endif
}

void displayInterpolate(DblRasterMx & mx)
{
	DblRasterMx  mx2;

	InterpolateRasterToSquares interpolator(mx, mx2);
	interpolator.interpolate();
	display(mx2);
}

void signStreamPixels(DblRasterMx & mxOp, DblRasterMx & mxRet, double mul)
{
	mxRet.initlike(mxOp);

	mfSignStream Obj(mxOp.begin(),mxOp.end(),mxRet.begin(),1,mfSignStream::typeFunctor(mul));

	_RUN(Obj)
}

void signStreamPixels(MultiflowDMatrix & mxLDD, DblRasterMx & mxRet,double mul)
{
	mxRet.initlike(mxLDD);

	mfSignStreamMLDD Obj(mxLDD.begin(),mxLDD.end(),mxRet.begin(),mfSignStreamMLDD::typeFunctor(mul));

	_RUN(Obj)
}

void signStreamPixels(DblRasterMx & mxOp,MultiflowDMatrix & mxLDD, DblRasterMx & mxRet,double mul)
{
	DblRasterMx  mxRet1;
	DblRasterMx  mxRet2;

	mxRet1.initlike(mxOp);
	mxRet2.initlike(mxOp);
	mxRet.initlike(mxOp);
	
	signStreamPixels(mxOp,mxRet1,mul);
	signStreamPixels(mxLDD,mxRet2,mul);
	
	DblRasterMx::iterator it1;
	DblRasterMx::iterator it2;
	DblRasterMx::iterator itRet;
	DblRasterMx::iterator end = mxRet1.end();

	for(it1 = mxRet1.begin(), it2 = mxRet2.begin(), itRet = mxRet.begin() ;
		it1!=end; ++it1, ++it2, ++itRet){
		
		if ( *it1==1.0 || *it2==1.0   ){
			*itRet = 1.0;
			continue;
		}

		if ( *it1==-1.0 || *it2==-1.0   ){
			*itRet = -1.0;
			continue;
		}
			
		*itRet = 0.0;
	}
}	

void windowAverage(DblRasterMx & mxOp, DblRasterMx & mxRet,double windowLength)
{
	mxRet.initlike(mxOp);
	mfWindowAvarage::typeFunctor functor(windowLength,mxOp.getPixelSize());
	mfWindowAvarage Obj(mxOp.begin(),mxOp.end(),mxRet.begin(),functor.getWindowRadius(),functor);

	_RUN(Obj)
}

void windowMin(DblRasterMx & mxOp, DblRasterMx & mxRet,double windowLength)
{
	mxRet.initlike(mxOp);
	mfWindowMin::typeFunctor functor(windowLength,mxOp.getPixelSize());
	mfWindowMin Obj(mxOp.begin(),mxOp.end(),mxRet.begin(),functor.getWindowRadius(),functor);

	_RUN(Obj)
}

void windowMax(DblRasterMx & mxOp, DblRasterMx & mxRet,double windowLength)
{
	mxRet.initlike(mxOp);
	mfWindowMax::typeFunctor functor(windowLength,mxOp.getPixelSize());
	mfWindowMax Obj(mxOp.begin(),mxOp.end(),mxRet.begin(),functor.getWindowRadius(),functor);

	_RUN(Obj)
}

void windowTotal(DblRasterMx & mxOp, DblRasterMx & mxRet,double windowLength)
{
	mxRet.initlike(mxOp);
	mfWindowTotal::typeFunctor functor(windowLength,mxOp.getPixelSize());
	mfWindowTotal Obj(mxOp.begin(),mxOp.end(),mxRet.begin(),functor.getWindowRadius(),functor);

	_RUN(Obj)
}

void windowVariance(DblRasterMx & mxOp, DblRasterMx & mxRet,double windowLength)
{
	mxRet.initlike(mxOp);
	mfWindowVariance::typeFunctor functor(windowLength,mxOp.getPixelSize());
	mfWindowVariance Obj(mxOp.begin(),mxOp.end(),mxRet.begin(),functor.getWindowRadius(),functor);

	_RUN(Obj)
}

void path( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet)
{
	
	if (mxLDD.getColNr()!=mx.getColNr() || mxLDD.getRowNr()!=mx.getRowNr())
		return; // this should throw an exception
	
	mxRet.initlike(mx);
	
	double initVal = 0.0;
	mxRet.fill(initVal);

	MarkTouchedRasters markFunc(mxRet, 1.0);

	DblRasterMx::iterator iterMx;
	DblRasterMx::iterator end = mx.end();
	MultiflowDMatrix::iterator iterMLDD;
	
	std::list<MultiflowDMatrix::iterator> minPoints;

	for(iterMx = mx.begin(), iterMLDD = mxLDD.begin();
		iterMx!=end; ++iterMx, ++iterMLDD){
		if ( *iterMx>0.0) 
			MLDDFunctions::walkOnFlow(iterMLDD,markFunc,minPoints);
	}
}

void catchment( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet)
{
	if (mxLDD.getColNr()!=mx.getColNr() || mxLDD.getRowNr()!=mx.getRowNr())
		return; // this should throw an exception
	
	mxRet.initlike(mx);
	
	double initVal = 0.0;
	mxRet.fill(initVal);

	MarkTouchedRasters markFunc(mxRet, 1.0);

	DblRasterMx::iterator iterMx;
	DblRasterMx::iterator end = mx.end();
	MultiflowDMatrix::iterator iterMLDD;
	
	std::list<MultiflowDMatrix::iterator> maxPoints;

	for(iterMx = mx.begin(),iterMLDD = mxLDD.begin() ;
		iterMx!=end; ++iterMx, ++iterMLDD){
		if ( *iterMx>0.0) 
			MLDDFunctions::walkBackOnFlow(iterMLDD,markFunc,maxPoints);
	}
}

void getMatrixPosItems(DblRasterMx & mxOp,RasterPositionMatrix & mxPos,DblRasterMx & mxTarget)
{
	size_t rows = mxOp.getRowNr();
	size_t cols = mxOp.getColNr();

	if (mxPos.getColNr()!=cols && mxPos.getRowNr()!=rows){
		//throw an exception
		return;
	}

	mxTarget.initlike(mxOp);
	
	
	RasterPositionMatrix::iterator iPos;
	DblRasterMx::iterator iTarget;

	for (iPos = mxPos.begin(), iTarget = mxTarget.begin();
		iPos!=mxPos.end(); ++iPos, ++iTarget){
		*iTarget = mxOp(iPos->getRow(),iPos->getCol());
	}
}

void diagonal(RasterPositionMatrix & mxPos,DblRasterMx & mxRet)
{
	mxRet.initlike(mxPos);

	mfDiagonalPosition Obj(mxPos.begin(), mxPos.end(),mxRet.begin(),mfDiagonalPosition::typeFunctor());
	_RUN(Obj)
}

void spreadLDD( MultiflowDMatrix & mxLDD, DblRasterMx & mxPoints, DblRasterMx & mxRet, double dblFluxLimitRate)
{
	SpreadLDD obj(mxLDD,mxPoints,mxRet,dblFluxLimitRate);
	obj.run();
}

void spreadLDDMax( MultiflowDMatrix & mxLDD, DblRasterMx & mxPoints, DblRasterMx & mxRet, double dblFluxLimitRate)
{
	SpreadLDDMax obj(mxLDD,mxPoints,mxRet,dblFluxLimitRate);
	obj.run();
}

//TODO: do it regular way!!!!!!!! (but is urgent now)
void sum_of_downstreamdiffs(MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet, double limitRate)
{
	size_t rows = mx.getRowNr();
	size_t cols = mx.getColNr();

	if (mxLDD.getRowNr()!=rows || mxLDD.getColNr()!=cols) {
		//TODO: throw exception here!
		return;
	}
	mxRet.initlike(mx);
	
	MultiflowDMatrix::iterator iLDD;
	DblRasterMx::iterator imx;
	DblRasterMx::iterator end = mx.end();
	DblRasterMx::iterator imxRet;
	double cardinalDom = 1/mx.getPixelSize();
	double diagonalDom = cardinalDom*SQRT2REC;
	double tmp = 0.0;

	for (iLDD=mxLDD.begin(), imx=mx.begin(), imxRet=mxRet.begin(); imx!= end; ++imx, ++iLDD,++imxRet) {

		double sum = 0.0;
        
		for (int i = 0; i < 9; i++) {
			if (i==5)
				continue;
			tmp = iLDD->getByChainCode(i);
			
			if (tmp>limitRate) {
				if (i==1 || i==3 || i==7 || i==9)
					sum+=(imx.chain_code(i) - *imx) * diagonalDom;
				else
					sum+=(imx.chain_code(i) - *imx) * cardinalDom;
			}
		}
		*imxRet=sum;
	}
}

void sum_of_upstreamdiffs(MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet, double limitRate)
{
	size_t rows = mx.getRowNr();
	size_t cols = mx.getColNr();

	if (mxLDD.getRowNr()!=rows || mxLDD.getColNr()!=cols) {
		//TODO: throw exception here!
		return;
	}
	mxRet.initlike(mx);
	
	MultiflowDMatrix::iterator iLDD;
	DblRasterMx::iterator imx;
	DblRasterMx::iterator end = mx.end();
	DblRasterMx::iterator imxRet;
	double cardinalDom = 1/mx.getPixelSize();
	double diagonalDom = cardinalDom*SQRT2REC;
	double tmp = 0.0;
    double currentVal = 0.0;
	double inCome = 0.0;
	double neighbourVal = 0.0;

	for (iLDD=mxLDD.begin(), imx=mx.begin(), imxRet=mxRet.begin(); imx!= end; ++imx, ++iLDD,++imxRet) {

		double sum = 0.0;
		double sumOfInCome = 0.0;
        currentVal = *imx;
		for (char i = 1; i < 10; i++) {
			if (i==5)
				continue;
			
			if (!iLDD.isValidItemByChainCode(i))
				continue;

			char my_chain_code = ::myChainCode(i);
			MultiflowDMatrix::iterator iNeighbour;

			if (!iLDD.neighbourIterator(i,iNeighbour))
				continue;
			
			tmp = iNeighbour->getByChainCode(my_chain_code);
			
			if (tmp > limitRate) {
				neighbourVal = mx(iNeighbour.getRow(), iNeighbour.getCol());
				inCome = neighbourVal * tmp;

				if (i==1 || i==3 || i==7 || i==9)
					sum+=(currentVal - neighbourVal) * diagonalDom * inCome;
				else
					sum+=(currentVal - neighbourVal) * cardinalDom * inCome;

				sumOfInCome+=inCome;
			}
		}
		*imxRet = sumOfInCome > 0.0 ? sum/sumOfInCome  : 0.0;
	}
}

bool compute_flux_distribution(MultiflowDMatrix & mxLDD, DblRasterMx & mxFlux, MultiflowDMatrix & mxRet)
{
	mxRet.initlike(mxLDD);
	DoubleChainCodeData initValue(0.0);
	mxRet.fill(initValue);

	if (mxLDD.getColNr()!=mxFlux.getColNr() || mxLDD.getRowNr()!=mxFlux.getRowNr())
		return false;

	MultiflowDMatrix::iterator iLDD = mxLDD.begin(), endLDD = mxLDD.end();
	DblRasterMx::iterator iFlux = mxFlux.begin();
	MultiflowDMatrix::iterator iRet = mxRet.begin();

	for (; iLDD != endLDD; ++iLDD, ++iFlux, ++iRet) {
		double currentFluid = *iFlux;
		for (char i = 1; i < 9; i++) {
			if (i==5)
				continue;
			if (!iLDD.isValidItemByChainCode(i))
				continue;
			
			double flux = currentFluid * iLDD->getByChainCode(i);
			iRet->setByChainCode(i, flux);
		}
	
	}

	return true;
}

void multiflowAngles(DblRasterMx & mxOp, MultiflowDMatrix & mxRet, bool bFillPits)
{
	mxRet.initlike(mxOp);
	DoubleChainCodeData initValue(0.0);
	mxRet.fill(initValue);
	MultiflowDMatrix::iterator iRet = mxRet.begin();
	if (bFillPits){
		DblRasterMx mx1;

#ifdef PARALELL_MODUL
		//FillPITs obj(mxOp,mx1,nThreadNr);
		FillPITs obj(mxOp,mx1,1);
#else	
		FillPITs obj(mxOp,mx1,1);
#endif
		obj.run();
		DblRasterMx::iterator iMx1 = mx1.begin(), endMx1 = mx1.end();
		
		MultiflowLDD Obj(1.0, iMx1 , endMx1 , iRet ,bFillPits,false, true);

		_RUN(Obj)
	}else{
		DblRasterMx::iterator iMx1 = mxOp.begin(), endMx1 = mxOp.end();

		MultiflowLDD Obj(1.0, iMx1 ,endMx1, mxRet.begin(),bFillPits,false, true);

		_RUN(Obj)
	}
}

double compute_velocity_mldd(MultiflowDMatrix & fluxDistribution, MultiflowDMatrix & flowAngles, double c,  MultiflowDMatrix & mxVelocity)
{
	mxVelocity.initlike(fluxDistribution);
	DoubleChainCodeData initValue(0.0);
	mxVelocity.fill(initValue);

	MultiflowDMatrix::iterator iFluxDist = fluxDistribution.begin(), endFluxDist = fluxDistribution.end();
	MultiflowDMatrix::iterator iAngles = flowAngles.begin();
	MultiflowDMatrix::iterator iRet = mxVelocity.begin();

	double time_interval = std::numeric_limits<double>::max();

	double cardinalLen = fluxDistribution.getPixelSize();
	double diagonalLen = cardinalLen * ::sqrt(2.0);
	for(; iFluxDist!=endFluxDist; ++iFluxDist, ++iAngles, ++iRet) {
		for (unsigned char cc = 1; cc < 10; ++cc) {
			if (cc==5)
				continue;
			if (!iFluxDist.isValidItemByChainCode(cc))
				continue;

			double angle = iAngles->getByChainCode(cc);
			double velocity = 0.0;
			if (angle > 0.0) {
				velocity = c * iFluxDist->getByChainCode(cc) * ::sin(angle);
			}
			iRet->setByChainCode(cc, velocity);

			if (velocity > 0.0) {
				double len = (cc==1 || cc==3 || cc==7 || cc==9) ? diagonalLen : cardinalLen;
				double max_time_interval = len/velocity;

				if (max_time_interval < time_interval)
					time_interval = max_time_interval;
			}

		
		}
	
	}

	return time_interval;

}

void compute_outflow_flux_mldd( MultiflowDMatrix & mxVelocity, MultiflowDMatrix & fluxDistribution, double dt, MultiflowDMatrix & mxRet)
{
	mxRet.initlike(mxVelocity);

	DoubleChainCodeData initValue(0.0);
	mxRet.fill(initValue);

	MultiflowDMatrix::iterator iVelocity = mxVelocity.begin(), endVelocity = mxVelocity.end();
	MultiflowDMatrix::iterator iFlux = fluxDistribution.begin();
	MultiflowDMatrix::iterator iRet = mxRet.begin();

	double cardinalLen = fluxDistribution.getPixelSize();
	double diagonalLen = cardinalLen * ::sqrt(2.0);

	for(; iVelocity!=endVelocity; ++iVelocity, ++iFlux, ++iRet) {
		for (unsigned char cc = 1; cc < 10; ++cc) {
			if (cc==5)
				continue;
			if (!iVelocity.isValidItemByChainCode(cc))
				continue;

			double velocity = iVelocity->getByChainCode(cc);
			double full_flux = iFlux->getByChainCode(cc);
			double len = (cc==1 || cc==3 || cc==7 || cc==9) ? diagonalLen : cardinalLen;
			double outflow_flux = (velocity * dt * full_flux) / len;
			iRet->setByChainCode(cc, outflow_flux); 
		}
	}


}

void compute_material_movement(MultiflowDMatrix & mxOutFlowFlux, DblRasterMx & mxRetInFlow, DblRasterMx & mxRetOutFlow)
{
	mxRetInFlow.initlike(mxOutFlowFlux);
	mxRetInFlow.fill(0.0);
	mxRetOutFlow.initlike(mxOutFlowFlux);
	mxRetOutFlow.fill(0.0);

	MultiflowDMatrix::iterator iOutFlow = mxOutFlowFlux.begin(), endOutFlow = mxOutFlowFlux.end();
	DblRasterMx::iterator iRetInFlow = mxRetInFlow.begin();
	DblRasterMx::iterator iRetOutFlow = mxRetOutFlow.begin();

	for (; iOutFlow!=endOutFlow ; ++iOutFlow, ++iRetInFlow, ++iRetOutFlow) {
		double sumOutflow = 0.0;
		for (unsigned char cc = 1; cc < 10; ++cc) {
			if (cc==5)
				continue;
			double outflow = iOutFlow->getByChainCode(cc);
			if (outflow > 0.0) {
				sumOutflow+=outflow;
				DblRasterMx::iterator cc_neighbour;
				iRetInFlow.neighbourIterator(cc, cc_neighbour);
				(*cc_neighbour)+=outflow;
			}
		}
		*iRetOutFlow = sumOutflow;
	}
}

void find_special_points(DblRasterMx & mx, unsigned int spec_points, IntRasterMx & ret)
{
	ret.initlike(mx);
	ret.fill(notSpecPoint);

	DblRasterMx::iterator imx = mx.begin(), endmx = mx.end();
	IntRasterMx::iterator iRet = ret.begin();
	int cnt = 0;
	for( ; imx != endmx; ++imx, ++iRet) {
		int nr_of_lower_local_mins = 0;
		int nr_of_higher_local_mins = 0;
		int nr_of_lower_local_maxs = 0;
		int nr_of_higher_local_maxs = 0;
		bool isPeak = true;
		double current_val = *imx;
		for (unsigned char cc = 1; cc < 10; ++cc) {
			if (cc==5)
				continue;
			
			unsigned char cc_next = cc==9 ? 1 : cc + 1;
			unsigned char cc_prev = cc==1 ? 9 : cc - 1;

			if (imx.isValidItemByChainCode(cc)) {
				double cc_val = imx.chain_code(cc);
				if (cc_val >= current_val) 
					isPeak = false;

				if (imx.isValidItemByChainCode(cc_next) && imx.isValidItemByChainCode(cc_prev)) {
					double cc_prev_val = imx.chain_code(cc_prev);
					double cc_next_val = imx.chain_code(cc_next);

					if (cc_prev_val < cc_val && cc_next_val < cc_val ) {
						if (cc_val > current_val)
							++nr_of_higher_local_maxs;
						else
							++nr_of_lower_local_maxs;
					}

					if (cc_prev_val > cc_val && cc_next_val > cc_val ) {
						if (cc_val > current_val)
							++nr_of_higher_local_mins;
						else
							++nr_of_lower_local_mins;
					}

				}
			}
		}

		int pixel_type = 0;
		/*
		if ((spec_points & ridge) && nr_of_local_mins > 1 && nr_of_local_maxs > 0)
			pixel_type |= ridge;

		if ((spec_points & peak) && isPeak)
			pixel_type |= peak;

		if ((spec_points & col) && nr_of_local_mins > 1 && nr_of_local_maxs == 2)
			pixel_type |= peak;
	    */
		if ((spec_points & ditch) && nr_of_higher_local_mins > 0 && nr_of_lower_local_mins > 0 && nr_of_higher_local_maxs > 1) {
			pixel_type |= ditch;	
			cnt++;
		}

		*iRet = pixel_type;
	}


}

void findOutflowPITs(DblRasterMx & mx,DblRasterMx & ret)
{
	ret.initlike(mx);
	ret.fill(0.0);
	DblRasterMx::iterator iMx = mx.begin(), end = mx.end();
	DblRasterMx::iterator iRet = ret.begin();
	for (; iMx != end; ++iMx, ++iRet) {
		if (iMx.isInOutflowPos()) {
			bool isPIT = true;
			double currentVal = *iMx;
			for (unsigned char cc = 1; cc < 10; ++cc) {
				double val;
				if (cc==5)
					continue;
				if (iMx.chain_code_safe(cc,val)) {
					if (val < currentVal) {
						isPIT = false;
						break;
					}
				}
			}
			if (isPIT) {
				*iRet = 1.0;	
			}
		}
	}
}


void compute_runoff_distribution( MultiflowDMatrix & mxVelocity, DblRasterMx & flowDepth, MultiflowDMatrix & mxRet)
{
	mxRet.initlike(mxVelocity);
	MultiflowDMatrix::iterator iVelocity = mxVelocity.begin(), endVelocity = mxVelocity.end();
	DblRasterMx::iterator iFlowDepth = flowDepth.begin();
	MultiflowDMatrix::iterator iRet = mxRet.begin();

	double pixelSize = mxVelocity.getPixelSize();
	for (; iVelocity!=endVelocity; ++iVelocity, ++iFlowDepth, ++iRet) {
		double depth = *iFlowDepth;
		for (int i = 0; i < 8; ++i) {
			(*iRet)(i) = (*iVelocity)(i) * depth * pixelSize; 
		}
	}
}


double compute_sediment_velocity_mldd(DblRasterMx & terrain, MultiflowDMatrix & runoff_distr, MultiflowDMatrix & mldd_slope, double runoff_exponent, double slope_exponent, double fluvial_const, double diffusive_const, double min_elevation_diff, MultiflowDMatrix & mxRet)
{
	mxRet.initlike(runoff_distr);
	DoubleChainCodeData initValue(0.0);
	mxRet.fill(initValue);

	MultiflowDMatrix::iterator iRunoff = runoff_distr.begin(), endRunoff = runoff_distr.end();
	MultiflowDMatrix::iterator iSlope = mldd_slope.begin();
	MultiflowDMatrix::iterator iRet = mxRet.begin();
	DblRasterMx::iterator iTerrain = terrain.begin();

	double max_time_interval = DoubleUtil::getMAXValue();

	for (; iRunoff != endRunoff; ++iRunoff, ++iSlope, ++iRet, ++iTerrain) {
		double current_terrain_value = *iTerrain;
		double velocity_sum = 0.0;
		for (int i = 0; i < 8; ++i) {
			double runoff = (*iRunoff)(i);
			double slope = (*iSlope)(i);
			double velocity = ::pow(runoff, runoff_exponent) * ::pow(slope, slope_exponent) * fluvial_const + diffusive_const*slope;
			(*iRet)(i) = velocity;
			velocity_sum+=velocity;
		}

		if (velocity_sum < 1e-10)
			continue;

		for (unsigned char cc = 1; cc < 10; ++cc) {
			if (cc==5)
				continue;
			if (!iTerrain.isValidItemByChainCode(cc))
				continue;
			double neighbour_terrain_value = iTerrain.chain_code(cc);
			if (current_terrain_value > neighbour_terrain_value + min_elevation_diff) {
				double velocity = iRet->getByChainCode(cc);
				double dt = (current_terrain_value - neighbour_terrain_value) / (velocity_sum + velocity);
				if (dt < max_time_interval)
					max_time_interval = dt;
			}
		}
	}

	return max_time_interval;
}

void compute_sediment_flux(MultiflowDMatrix & mxSedimentVelocityMLDD, double dt, MultiflowDMatrix & mxRet)
{
	mxRet.initlike(mxSedimentVelocityMLDD);

	MultiflowDMatrix::iterator iSedimentVelocity = mxSedimentVelocityMLDD.begin(), endSedimentVelocity = mxSedimentVelocityMLDD.end();
	MultiflowDMatrix::iterator iRet = mxRet.begin();

	for (; iSedimentVelocity != endSedimentVelocity; ++iSedimentVelocity, ++iRet) {
		for (int i = 0; i < 8; ++i) {
			(*iRet)(i) = (*iSedimentVelocity)(i) * dt;
		}
	}
}


bool loadFromArcgis(const char * lpszFileName, DblRasterMx & mx)
{
	std::ifstream isf(lpszFileName);

	if (!isf.is_open())
	{
		return false;
	}

	std::map<std::string, double> attributeMap;

	for (int i = 0; i < 6; ++i) {
		std::string name;
		double val;
		isf >> name;
		isf >> val;

		if (name.empty())
			continue;

		attributeMap[name] = val;
	}

	size_t rows = static_cast<size_t>(attributeMap["nrows"]);
	size_t cols = static_cast<size_t>(attributeMap["ncols"]);
	double pixelSize = attributeMap["cellsize"];

	if (rows==0 || cols==0 || pixelSize <= 0.0)
		return false;

	double initVal = 0.0;
	mx.init(rows, cols, pixelSize, origoBottomLeft, initVal);
	size_t pixel_nr = rows * cols;

	DblRasterMx::iterator iter = mx.begin(), end = mx.end();

	size_t cnt = 0;
	for (; iter != end && !isf.eof(); ++iter, ++cnt) {
		double val;
		isf >> val;
		*iter = val;
	}

	if (cnt != pixel_nr)
		return false;

	return true;
}

}