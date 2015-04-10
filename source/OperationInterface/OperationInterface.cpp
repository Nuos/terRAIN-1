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

	if (bFillPits){
		DblRasterMx mx1;

#ifdef PARALELL_MODUL
		FillPITs obj(mxOp,mx1,1);
		//FillPITs obj(mxOp,mx1,nThreadNr);
#else	
		FillPITs obj(mxOp,mx1,1);
#endif
		obj.run();

		mfLDD Obj(mx1.begin(),mx1.end(),mxRet.begin(),1,mfLDD::typeFunctor());

		_RUN(Obj)
	}else{
		mfLDD Obj(mxOp.begin(),mxOp.end(),mxRet.begin(),1,mfLDD::typeFunctor());
		_RUN(Obj)
	}
}

void findpits(  DblRasterMx & mxOp, DblRasterMx & mxRet )
{
	mxRet.initlike(mxOp);
	
	mfFindPITs Obj(mxOp.begin(),mxOp.end(),mxRet.begin(),1,mfFindPITs::typeFunctor());

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

	

	if (bFillPits){
		DblRasterMx mx1;

#ifdef PARALELL_MODUL
		//FillPITs obj(mxOp,mx1,nThreadNr);
		FillPITs obj(mxOp,mx1,1);
#else	
		FillPITs obj(mxOp,mx1,1);
#endif
		obj.run();

		MultiflowLDD Obj(1.0, mx1.begin(), mx1.end(), mxRet.begin(),bFillPits,true);

		_RUN(Obj)
	}else{
		MultiflowLDD Obj(1.0, mxOp.begin(), mxOp.end(), mxRet.begin(),bFillPits,true);

		_RUN(Obj)
	}
}

void multiflowLDD(double a, DblRasterMx & mxOp, MultiflowDMatrix & mxRet, bool bFillPits)
{
	mxRet.initlike(mxOp);

	

	if (bFillPits){
		DblRasterMx mx1;

#ifdef PARALELL_MODUL
		//FillPITs obj(mxOp,mx1,nThreadNr);
		FillPITs obj(mxOp,mx1,1);
#else	
		FillPITs obj(mxOp,mx1,1);
#endif
		obj.run();

		MultiflowLDD Obj(a, mx1.begin(), mx1.end(), mxRet.begin(),bFillPits,false);

		_RUN(Obj)
	}else{
		MultiflowLDD Obj(a, mxOp.begin(), mxOp.end(), mxRet.begin(),bFillPits,false);

		_RUN(Obj)
	}

}

void multiflowLDD(DblRasterMx a, DblRasterMx & mxOp, MultiflowDMatrix & mxRet, bool bFillPits)
{
	mxRet.initlike(mxOp);

	

	if (bFillPits){
		DblRasterMx mx1;

#ifdef PARALELL_MODUL
		//FillPITs obj(mxOp,mx1,nThreadNr);
		FillPITs obj(mxOp,mx1,1);
#else	
		FillPITs obj(mxOp,mx1,1);
#endif
		obj.run();

		MultiflowLDD Obj(mx1.begin(), mx1.end(), mxRet.begin(),bFillPits,false,a.begin());

		_RUN(Obj)
	}else{
		MultiflowLDD Obj(mxOp.begin(), mxOp.end(), mxRet.begin(),bFillPits,false,a.begin());

		_RUN(Obj)
	}
}

void check_loop( DblRasterMx & mxLDD, DblRasterMx & mxRet)
{
	DblRasterMx mxDepth;
	
	mxRet.initlike(mxLDD);
	
	double dblTmp = 0.0;
	mxRet.fill(dblTmp);

	loopCheckFuncOnLDD Obj(mxLDD.begin(), mxLDD.end(),mxDepth, mxRet );

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
		for (char i = 0; i < 9; i++) {
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

}