#include "FixedTimeStepRunFallRunOff.h"
#include "OperationInterface.h"
#include "StatFile.h"
namespace SIMULATION 
{

FixedTimeStepRunFallRunOff::FixedTimeStepRunFallRunOff(ostream & os, ParamHandler & params)
:Simulation(os, params)
{}

bool FixedTimeStepRunFallRunOff::run()
{
	setOutflowType(ofBottomLeft);
	size_t nSizeX = 20;
	size_t nSizeY = 20;
	
	StatFile statFile("statfile");

	double endOfRain = 4;
	double rain = 0;

    // number of iterations:
	size_t iterationNumber = 200;
	   
	DblRasterMx mx0;
	DblRasterMx mx1;
	DblRasterMx mx3;
	DblRasterMx tst1;
	DblRasterMx tst2;
	DblRasterMx tst3;
	DblRasterMx zero;
	DblRasterMx mxEdges;
	DblRasterMx mxRandom;
	DblRasterMx mxSlope;
	DblRasterMx flow;
	DblRasterMx mxFluid;
	DblRasterMx flowOut;
	DblRasterMx flowIn;
	DblRasterMx mxLDD;
	MultiflowDMatrix  mxMLDD;
	DblRasterMx mxDiagonal;
	DblRasterMx mxVelocity;

	   

	mapattr(nSizeY,nSizeX,10,100, mx0);
	mapattr(nSizeY,nSizeX,10,0, mx3);
	mapattr(nSizeY,nSizeX,10,100, mxRandom);
	mapattr(nSizeY,nSizeX,10,100, tst1);
	mapattr(nSizeY,nSizeX,10,100, tst2);
	mapattr(nSizeY,nSizeX,10,0, tst3);
	mapattr(nSizeY,nSizeX,10,0, zero);
	mapattr(nSizeY,nSizeX,10,0.0, flow);
	mapattr(nSizeY,nSizeX,10,0.0, flowOut);
	mapattr(nSizeY,nSizeX,10,0.0, flowIn);
	mapattr(nSizeY,nSizeX,10,1, mxFluid);
	mapattr(nSizeY,nSizeX,10,0, mxDiagonal);

	mapattr(nSizeY,nSizeX,10,0.5, mxVelocity);


	uniform(mxRandom);
	add(mx0,mxRandom,mx1);
	  
	saveToAsc(mx1,0,"startTerrain");

	size_t i = 1;
	size_t j = 0;
	size_t l = 0;

	multiflowLDD( 1, mx1, mxMLDD,true);
	diagonal(mxMLDD, mxDiagonal);
	
	   
	mxDiagonal(19,0)=1.0;
	double sumOfOutFlow = 0.0;

	for (i = 1; i <= iterationNumber; i++) {

			std::cout<<"Iteration nr: "<<i<<std::endl;
		  				
			sumOfOutFlow+=flow(19,0);
			statFile.print(flow(19,0));
			statFile.endl();

			if (i <= endOfRain){
				rain=1;
			}else{
				rain=0;
			}

			mapattr(nSizeY,nSizeX,10,rain, mxFluid);				 	   
			upstreamtotal(mxMLDD, flow, flowIn, true, true);
			
			/*		
			flowOut = flow * mxDiagonal ;
			flow = flow - flowOut + flowIn + mxFluid;
			*/
			
			flowOut = flow /* * mxDiagonal * mxVelocity*/; 
			flowIn = flowIn /* * mxVelocity*/;
			flow = flow - flowOut + flowIn + mxFluid;
			
			if (( i % 1 )==0){
				saveToAsc(flow,i,"flow");
				saveToAsc(flowOut,i,"flowOut");
				saveToAsc(flowIn,i,"flowIn");
				saveToAsc(mxDiagonal, i, "mxDiagonal");
			}	       
	}

	statFile.print("sum: ");
	statFile.print(sumOfOutFlow);

	saveToAsc(mx1,0,"result");
	printmx(mx1);

	return true;
}

}