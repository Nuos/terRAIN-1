#include "SimpleErosion.h"

#include "OperationInterface.h"
#include "rastermatrix.h"
#include "Timer.h"
#include "StatFile.h"

#ifdef VISUALIZATION
#include "TerrainRenderWindow.h"
#endif


#include "FillPITs.h"

namespace SIMULATION
{

SimpleErosion::SimpleErosion(ostream & os, ParamHandler & params):
	Simulation(os,params)
{}

	
bool SimpleErosion::run()
{
	

	// statfile:
    StatFile statFile("statfile");
    
	// in the accuflux function neglet the flow if account is less then
    // initial flow account*accuFluxLimitRate
    double accuFluxLimitRate = 0.01;
    double rasterShapeFactor = 1;
    // double limitUpstream = 0.01;
    // decrease the boundary pixels in every iteration with this value:
    // double boundDecrease = 0.01;
    // double Kfluv = 0.1;
    double rain = 1.0;
    //double Susp = 0;          // 1 for TRL, 0 for DTL
    // number of iterations:

    DblRasterMx mx0;
    DblRasterMx mx1;
    DblRasterMx mx3;
    DblRasterMx tst2;
    DblRasterMx tst3;
    DblRasterMx zeroPlusABit;
    DblRasterMx mxEdges;
    DblRasterMx mxRandom;
	DblRasterMx mxSlope;
	DblRasterMx mxSlopeCorrected;
	DblRasterMx mxAccflux;
	DblRasterMx mxFluid;
	DblRasterMx change;
	DblRasterMx mxDownstreamMax;
	DblRasterMx mxUpstreamMin;
	DblRasterMx lowerLimit;
	DblRasterMx upperLimit;
	DblRasterMx sedOut;
	DblRasterMx sedIn;
	//DblRasterMx DiffSedOut;
	//DblRasterMx DiffSedIn;
	DblRasterMx mxEroded;
	DblRasterMx susp;
	DblRasterMx boundDecrease;
	DblRasterMx limitUpstream;
	DblRasterMx m;
	DblRasterMx n;
	DblRasterMx kDiff;
	DblRasterMx kFluv;
	DblRasterMx mxLDD;
	MultiflowDMatrix  mxMLDD;
	DblRasterMx mxDiagonal;
	RasterPositionMatrix dummyRasterPosMx;

	size_t rows = 100;
	size_t cols = 100;

	mapattr(rows,cols,10,100, mx0);
	mapattr(rows,cols,10,0, mx3);
	mapattr(rows,cols,10,0, mxSlopeCorrected);
	mapattr(rows,cols,10,0, mxSlope);
	mapattr(rows,cols,10,100, tst2);
	mapattr(rows,cols,10,10, tst3);
	mapattr(rows,cols,10,0.00001, zeroPlusABit);
	mapattr(rows,cols,10,100, mxEdges);
	mapattr(rows,cols,10,0.0, mxRandom);
	mapattr(rows,cols,10,0.0, mxDownstreamMax);
	mapattr(rows,cols,10,0.0, mxUpstreamMin);
	mapattr(rows,cols,10,0.0, mxEroded);
	mapattr(rows,cols,10,0.0, sedOut);
	mapattr(rows,cols,10,0.0, sedIn);
	mapattr(rows,cols,10,0.0, change);
	//mapattr(nSizeY,nSizeX,10,0.0, DiffSedOut);
	//mapattr(nSizeY,nSizeX,10,0.0, DiffSedIn);
	mapattr(rows,cols,10,0.0, lowerLimit);
	mapattr(rows,cols,10,0.0, upperLimit);
	mapattr(rows,cols,10,1, susp);
	mapattr(rows,cols,10,0.01, boundDecrease);
	mapattr(rows,cols,10,0.1, limitUpstream);
	mapattr(rows,cols,10,1.2, m);
	mapattr(rows,cols,10,1.6, n);
	mapattr(rows,cols,10,0.01, kDiff);
	mapattr(rows,cols,10,0.001, kFluv);
	mapattr(rows,cols,10,rain, mxFluid);
	mapattr(rows,cols,10,0, mxDiagonal);

	size_t i = 1;
	size_t j = 0;
	size_t l = 0;

	for ( j = 0; j < cols; j++ ){
		for ( l = 0; l < rows; l++ ){
		   mx0(l,j) = 100 - l * 0.0;
		}
    }

    uniform(mxRandom);
	
	DblRasterMx mxx;
    mxx = mx0 + mxRandom * tst3;
	
	FillPITs obj(mxx,mx1,1);
	
	obj.run();
	
	display(mx1);
	
	saveToAsc(mx1,0,"startTerrain");

	Timer timeLoop;
	timeLoop.start();
	Timer timeLDDCreate;
	Timer timeAccFlux;
	Timer timeSlope;
	Timer timeErosion;

	//loadFromAsc("d:\\pcrinst\\tst2.asc",mx3,10);
	//loadFromAsc("d:\\terRAIN\\test\\mx1010000.asc",mx1,10);
	//loadFromAsc("d:\\pcrinst\\mx1010000.asc",mx1,10);
	//printmx(mx3);
	mxEdges=mx1;
	size_t iterationNumber = 100;
	for (i = 1; i <= iterationNumber; i++) {

		_os<<"Iteration nr: "<<i<<std::endl;

		// a mátrix átlaga:
		statFile.print(mx1);
		// új sort kezd:
		statFile.endl();

		timeSlope.start();
		slope(mx1,mxSlope);
		timeSlope.stop();
		timeLDDCreate.start();
		multiflowLDD( 1, mx1, mxMLDD,true);
		timeLDDCreate.stop();
		timeLDDCreate.printLastPeriod("multiflowLDD: ",_os);
	
		timeAccFlux.start();
		accflux(mxMLDD,mxFluid,mxAccflux,accuFluxLimitRate);
		timeAccFlux.stop();
		timeAccFlux.printLastPeriod("accflux: ",_os);
		timeErosion.start();
		downstreammax( mxMLDD, mx1, mxDownstreamMax,dummyRasterPosMx,false);
		mxUpstreamMin = mx1 + limitUpstream;
		max(mxSlope,zeroPlusABit, mxSlopeCorrected);
		diagonal(mxMLDD, mxDiagonal);
		sedOut = kDiff*mxSlopeCorrected + kFluv*(mxSlopeCorrected^n)*(mxAccflux^m);
	
		upstreamtotal(mxMLDD, sedOut, sedIn, true,true);
		change = sedIn*susp - sedOut*mxDiagonal;
		mx1 = mx1 + change;
	
		if (i < 100){
			for ( j = 0; j < cols; j++ ){
			   for ( l = 0; l < rows; l++ ){
					   boundDecrease(l,j)= 1;
			   }
			}
		}else{
			for ( j = 0; j < cols; j++ ){
				for ( l = 0; l < rows; l++ ){
					boundDecrease(l,j)= 0;
				}
			}
		}

		mxEdges = mxEdges-boundDecrease;

		//// lower Edges
		for ( j = 0; j < rows; j++ ){
			mx1(j,cols-1) = mxEdges(j,cols-1);
			mx1(j,0) = mxEdges(j,0);
		}

		for ( j = 1; j < cols-1; j++ ){
			mx1(rows-1,j) = mxEdges(rows-1,j);
			mx1(0,j) = mxEdges(0,j);
		}


		timeErosion.stop();

		if (( i % 5000 )==0){
			saveToAsc(mx1,i,"mx1");
			saveToAsc(mxAccflux,i,"accflux");
			saveToAsc(mxSlopeCorrected,i,"slope");
			saveToAsc(sedOut, i, "sedOut");
			saveToAsc(sedIn, i, "sedIn");
			saveToAsc(change, i, "change");
			saveToAsc(mxLDD, i, "mxLDD");
			saveToAsc(mxMLDD, i, "mxMLDD");
			saveToAsc(mxDiagonal, i, "mxDiagonal");
			
			//RenderWindowParams winparams(300,400);
			//TerrainRenderWindow win(winparams);
			//win.setTerrain(mx1);
			//win.display();
		}

      }

      saveToAsc(mx1,0,"result");
      printmx(mx1);
	   
	 // RenderWindowParams winparams(300,400);
	 // TerrainRenderWindow win(winparams);
	 // win.setTerrain(mx1);
	 // win.display();
		
	  display(mx1);
	  displayInterpolate(mx1);
	  timeLoop.stop();
      timeSlope.print("Slope: ",_os);
      timeLDDCreate.print("LDDCreate: ",_os);
      timeAccFlux.print("AccFlux: ",_os);
      timeErosion.print("Erosion: ",_os);
      timeLoop.print("Loop duration in seconds: ",_os);

      return 0;
	
}

}