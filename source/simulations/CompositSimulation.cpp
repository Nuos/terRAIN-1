#include "CompositSimulation.h"
#include "SimpleRunoff.h"
#include "FillMLDDPits.h"
#include "OperationInterface.h"
#include "StatFile.h"
#include "FillMLDDPits.h"
#include "MarkTouchedRasters.h"
#include "MLDDFunctions.h"
#include <iostream>

#include "defs.h"
#include "FillPITs.h"
#include "InterpolateRasterToSquares.h"
#include "DoubleUtil.h"

using namespace std;
using namespace TR;

namespace SIMULATION
{

CompositSimulation::CompositSimulation(ostream & os, ParamHandler & params) :
	Simulation(os,params)
{
}


enum SoilProductionType
{
	gpNone = 0,
	gpExponential
};

enum RunoffProductionType
{
	rfRainfallRunoff = 0,
	rfCatchmentBasedEstimation
};

enum SedimentTransportType
{
	stTotalRemoval = 0,
	stPixelPixelTransport
};

/*
bool symm(DblRasterMx & mx)
{
	size_t rows = mx.getRowNr();
	size_t cols = mx.getColNr();

	size_t s = cols/2;

	for (size_t row = 0; row < rows; ++row) {
		for (size_t col = 0; col < s; ++col) {
			double v1 = mx(row, col);
			double v2 = mx(row, cols - col - 1);
			if (fabs(v1 - v2) > 1e-6)
				return false;
		}
	}

	return true;
}


bool symm(MultiflowDMatrix & mx)
{
	size_t rows = mx.getRowNr();
	size_t cols = mx.getColNr();

	size_t s = cols/2;

	for (size_t row = 0; row < rows; ++row) {
		for (size_t col = 0; col < s; ++col) {
			DoubleChainCodeData & d1 = mx(row, col);	
			DoubleChainCodeData & d2 = mx(row, cols - col - 1);

			if (fabs(d1.getByChainCode(1) - fabs(d2.getByChainCode(3)))> 1e-6){
				return false;
			}

			if (fabs(d1.getByChainCode(2) - fabs(d2.getByChainCode(2)))> 1e-6){
				return false;
			}

			if (fabs(d1.getByChainCode(3) - fabs(d2.getByChainCode(1)))> 1e-6){
				return false;
			}

			if (fabs(d1.getByChainCode(4) - fabs(d2.getByChainCode(6)))> 1e-6){
				return false;
			}

			if (fabs(d1.getByChainCode(6) - fabs(d2.getByChainCode(4)))> 1e-6){
				return false;
			}

			if (fabs(d1.getByChainCode(7) - fabs(d2.getByChainCode(9)))> 1e-6){
				return false;
			}

			if (fabs(d1.getByChainCode(8) - fabs(d2.getByChainCode(8)))> 1e-6){
				return false;
			}

			if (fabs(d1.getByChainCode(9) - fabs(d2.getByChainCode(7)))> 1e-6){
				return false;
			}

		}
	}

	return true;
}

*/
bool CompositSimulation::run()
{
	setOutputDirectory("d:\\terrain_output3");
	//setOutputDirectory("e:\\Solyom\\run3");
	// simulation settings
	//setOutflowType(ofAllSides);
	//setOutflowType(ofTopAndBottomSide);
	setOutflowType(ofLeftAndRightSide);
	
	SoilProductionType groundProductionType = gpNone;
	
	//rfCatchmentBasedEstimation rfRainfallRunoff
	RunoffProductionType runoffProductionType = rfCatchmentBasedEstimation;
	
	//stTotalRemoval stPixelPixelTransport
	SedimentTransportType sedimentTransportType = stPixelPixelTransport;

	double rainTime = 1000;
	double logTimeInc = 10;
	double max_iteration_time = 1;
	double min_iteration_time = 0.0000001;
	
	erosion_rate_params erosionRateParams;
	erosionRateParams.critical_slope = M_PI/2/2;
	erosionRateParams.infinite_erosion_rate = 1;
	erosionRateParams.diffusion_exponent = 2.0;
	erosionRateParams.diffusive_const = 0.0; //0.0001;	
	erosionRateParams.fluvial_const = 0.001; //0.001;
	erosionRateParams.min_elevation_diff = 1e-7;
	erosionRateParams.runoff_exponent = 1.5;
	erosionRateParams.slope_exponent = 1.5;
	erosionRateParams.simple_diffusion = true;

	double kTect = 0.1;
	double kTectLeft = 0.0;
	double kTectRight = 0.0;
	double kHor = 0.0;
	double kTectSpread = 1.0;

	// common variables
	size_t nSizeX = 40;
	size_t nSizeY = 20;
	size_t EdgesLimit = 9;
	double pixelSize = 10;
	double rainIntensity = 1; // [length_unit/time_unit]
	double iteration_time = max_iteration_time; // [time_unit]
	double accumulated_rain = 0.0; //[length_unit] do not change
	double pathsum = 0;
	double pitsum = 0;
	
	DblRasterMx pixelSizeMatrix;
	DblRasterMx mx1;
	DblRasterMx mxSlope;
	DblRasterMx mxSlopeCorrected;
	DblRasterMx zeroPlusABit;
	DblRasterMx five;
	DblRasterMx mxAccflux;
	DblRasterMx mxLongest;
	DblRasterMx logMxLongest;
	DblRasterMx logMxAccflux;
	DblRasterMx elongation;
	DblRasterMx upstreamMxDiagonal;
	DblRasterMx mxUpstreamMax;
	DblRasterMx mxDownstreamMax;
	DblRasterMx mxDownstreamRatio;
	DblRasterMx dAdL;
	DblRasterMx dAdLA;
	DblRasterMx mxDiagonal;
	DblRasterMx streams;
	DblRasterMx path1;
	DblRasterMx mxSedInDiffusive;
	DblRasterMx mxSedOutDiffusive;
	DblRasterMx mxSedInFluvial;
	DblRasterMx mxSedOutFluvial;
	DblRasterMx Edges;
	DblRasterMx EdgesRight;
	DblRasterMx EdgesLeft;
	DblRasterMx xCoord;
	DblRasterMx yCoord;
	DblRasterMx pits;
	DblRasterMx bedloadRatio;
	DblRasterMx kTectMxIteration;

	MultiflowDMatrix mxSMLDD;
	MultiflowDMatrix terrainMLDD;

	RasterPositionMatrix upstreamMaxAreaPosition;
	RasterPositionMatrix downstreamMaxAreaPosition;

	mapattr(nSizeY,nSizeX,pixelSize,pixelSize, pixelSizeMatrix);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mx1);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxSlope);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxSlopeCorrected);
	mapattr(nSizeY,nSizeX,pixelSize,0.0001, zeroPlusABit);
	mapattr(nSizeY,nSizeX,pixelSize,5.0, five);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxAccflux);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxLongest);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, logMxAccflux);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, logMxLongest);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, elongation);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxUpstreamMax);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxDownstreamMax);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxDownstreamRatio);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, upstreamMxDiagonal);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, dAdL);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, dAdLA);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxDiagonal);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, streams);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, path1);
	mapattr(nSizeY,nSizeX,pixelSize,100, Edges);
	mapattr(nSizeY,nSizeX,pixelSize,100, EdgesRight);
	mapattr(nSizeY,nSizeX,pixelSize,100, EdgesLeft);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, xCoord);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, yCoord);
	mapattr(nSizeY,nSizeX,pixelSize,0.8, bedloadRatio);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, kTectMxIteration);
	
	DblRasterMx mxRain;
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxRain);
	DblRasterMx mxFlowDepth;
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxFlowDepth);
	double elapsedTime = 0.0;
	double elapsedRainTime = 0.0;
	// base terrain layer (rock based)
	DblRasterMx rock;
	// graound layer above the rock based terrain layer 
	DblRasterMx soil;

		

	//initial terrain and ground 
	switch (groundProductionType)
	{
		case gpNone:
		{
			mapattr(nSizeY,nSizeX,pixelSize,0.0, rock);
			// initial ground generation
			
			double elevation = 100;
			
			DblRasterMx randomNoise;
			DblRasterMx multiplicatorSmall;
			mapattr(nSizeY,nSizeX,pixelSize,0.0, randomNoise);
			mapattr(nSizeY,nSizeX,pixelSize,1, multiplicatorSmall);
			mapattr(nSizeY,nSizeX,pixelSize,elevation, rock);
			uniform(randomNoise);
			
			mapattr(nSizeY,nSizeX,pixelSize,0.0, soil);
			//saveToArcgis(randomNoise, 0, "randomNoise");

			xcoordinate(rock, xCoord);
			ycoordinate(rock, yCoord);
			int j = 0;
			int l = 0;
			/*for ( j = 0; j < nSizeX; j++ ){		
						for ( l = 0; l < nSizeY; l++ ){			
							if (j < 10){
								rock(l,j) = 100 + xCoord(l,j) * 0.001;                 
							} else {
								rock(l,j) = 100 + 0.2 - xCoord(l,j) * 0.001;
							}
						}
			   }*/
			rock = rock + randomNoise * multiplicatorSmall;
			
			/*if (!loadFromArcgis("d:\\terrain_output2\\mx1000010.asc",rock)) {
				std::cout << "Unable to read arc gis file" << std::endl;
				return false;
			}*/

			//findpits(rock, pits);
			//saveToArcgis(pits, 0, "pits");

			IntRasterMx terrain_pixel_types;
			find_special_points(rock, channel, terrain_pixel_types);
			saveToArcgis(terrain_pixel_types, 0, "special_points_slope");
			break;
			
		}
		default:
			std::cout << "Invalid ground type production type: " << groundProductionType;
			return false;
	}
	DblRasterMx terrain;
    terrain = rock + soil;
	printmx(terrain);

	DblRasterMx outflowPITs;
	findOutflowPITs(terrain, outflowPITs); 

	bool stopCondition = true;

	// params of rfRainfallRunoff
	double velocity_coefficent = 10.0;

	// params of rfCatchmentBasedEstimation
	DblRasterMx mxFluid;
	mapattr(nSizeY,nSizeX,pixelSize,pixelSize*pixelSize*rainIntensity, mxFluid);


	size_t iteration_nr = 0;
	double waterOnPits = 0.0;
	//double logTimeInc = 10;
	double nextLogTime = 0.0;
	int log_index = 1;

	
	double shiftTimeInc = kHor > 0 ? pixelSize/kHor : DoubleUtil::getMAXValue();
	double nextShiftTime = shiftTimeInc;
	double shiftTimeInc2 = kTectSpread > 0 ? pixelSize/kTectSpread : DoubleUtil::getMAXValue();
	double nextShiftTime2 = shiftTimeInc2;

	double UpliftTimeInc = kTectSpread > 0 ? pixelSize/kTectSpread : DoubleUtil::getMAXValue();
	double nextUpliftTime = UpliftTimeInc;
	size_t nextUpliftCol = 0;
	
	StatFile statFile("statfile.txt",15);
	statFile.print("elapsed time");
	statFile.print("iteration time");
	statFile.endl();
	StatFile statFile2("statfile2.txt",15);
	statFile2.print("pathsum");
	statFile2.print("pitsum");
	statFile2.print("EdgesLimit");
	statFile2.endl();
	
	while (stopCondition) 
	{
		bool redo_iteration = true;
		while (redo_iteration) {
			DblRasterMx copyOfFlowDepth = mxFlowDepth;
			double copy_of_accumulated_rain = accumulated_rain;
			double copy_of_elapsedRainTime = elapsedRainTime;

			MultiflowDMatrix runoff_distribution;
			
			switch (runoffProductionType)
			{
				case rfRainfallRunoff:
				{
					double rain_time_within_iteration = min(iteration_time, rainTime - copy_of_elapsedRainTime);
					double rain = rain_time_within_iteration * rainIntensity;
					bool isRain = false;
					if (rain > 0.0) {
						for (size_t i = 0; i < nSizeY; i++){
							for (size_t j = 0; j < nSizeX; j++){
								mxRain(i,j)=rain;	
							}
						}
						copyOfFlowDepth = copyOfFlowDepth + mxRain;
						copy_of_accumulated_rain += rain*pixelSize*pixelSize*nSizeX*nSizeY;
						copy_of_elapsedRainTime+=rain_time_within_iteration; 
						isRain = true;
					}
					DblRasterMx waterSurface;
					waterSurface = terrain + copyOfFlowDepth;

					MultiflowDMatrix  mxWaterSurfaceAngles;
					multiflowAngles(waterSurface, mxWaterSurfaceAngles, true);

					// mldd of water surface
					MultiflowDMatrix  waterMLDD;
					multiflowLDD( 1.0, waterSurface, waterMLDD, true);

					MultiflowDMatrix flux_distribution;
					compute_flux_distribution(waterMLDD, copyOfFlowDepth, flux_distribution);

					MultiflowDMatrix velocity_mldd;
					double time_interval = compute_velocity_mldd(flux_distribution, mxWaterSurfaceAngles, velocity_coefficent,  velocity_mldd);
					time_interval = ::min(time_interval, max_iteration_time);
					iteration_time = ::min(iteration_time, time_interval);
					MultiflowDMatrix mxOutflowFlux;
					compute_outflow_flux_mldd( velocity_mldd, flux_distribution, time_interval,mxOutflowFlux);

					DblRasterMx mxInFlow;
					DblRasterMx mxOutFlow;
					compute_material_movement(mxOutflowFlux,mxInFlow, mxOutFlow);
					copyOfFlowDepth = copyOfFlowDepth - mxOutFlow + mxInFlow;

					DblRasterMx::iterator iOutFlowPITs = outflowPITs.begin(), endOutFlowPITS = outflowPITs.end();
					DblRasterMx::iterator iFlowDepth = copyOfFlowDepth.begin();
				
					for (; iOutFlowPITs!=endOutFlowPITS; ++iOutFlowPITs, ++iFlowDepth) {
						if (*iOutFlowPITs > 0.0) {
							waterOnPits+= *iFlowDepth * pixelSize * pixelSize;
							*iFlowDepth = 0.0;
						}
					}
					compute_runoff_distribution( velocity_mldd,copyOfFlowDepth,runoff_distribution);
					// check only if there is no more rainfall
					if (!isRain) {
						std::cout << "volume of fluid in outflow position: = " << waterOnPits << std::endl;
						if ( fabs(waterOnPits - copy_of_accumulated_rain) < DoubleUtil::sDeltaD3) {
							stopCondition = false;
						}
					}
					//end of check	
				}
				break;
				case rfCatchmentBasedEstimation:
				{
					MultiflowDMatrix  terrainMLDD;
					multiflowLDD( 1	, terrain, terrainMLDD, false);
					DblRasterMx mxAccflux;
					accflux(terrainMLDD,mxFluid,mxAccflux,0.0);
					compute_flux_distribution(terrainMLDD, mxAccflux, runoff_distribution);
					
					if ( elapsedTime >= rainTime) {
						stopCondition = false;
					}
				}
				break;
			}

			MultiflowDMatrix  mxMLLDSlope;
			multiflowAngles(terrain, mxMLLDSlope, false);
			erosion_rate_results erosion_rates;
			double max_time_interval_of_sediment_flow = 0.8 *compute_erosion_rate(erosionRateParams, terrain, runoff_distribution, mxMLLDSlope, erosion_rates);

			max_time_interval_of_sediment_flow = ::min(max_time_interval_of_sediment_flow, max_iteration_time);
			max_time_interval_of_sediment_flow = ::max(max_time_interval_of_sediment_flow, min_iteration_time);
			switch (runoffProductionType)
			{
				case rfRainfallRunoff:
					if (iteration_time > max_time_interval_of_sediment_flow) {
						iteration_time = max_time_interval_of_sediment_flow;
						continue;
					}
				break;
				case rfCatchmentBasedEstimation:
					iteration_time = max_time_interval_of_sediment_flow;
				break;
			}
			
			MultiflowDMatrix mxSedimentMovementFluvial;
			compute_sediment_flux(erosion_rates.mxFluvialErosionRate, iteration_time, mxSedimentMovementFluvial); 

			compute_material_movement(mxSedimentMovementFluvial,mxSedInFluvial, mxSedOutFluvial);

			rock = rock - mxSedOutFluvial;

			if (sedimentTransportType == stPixelPixelTransport) {
				rock = rock + (mxSedInFluvial * bedloadRatio);
			}
			

			MultiflowDMatrix mxSedimentMovementDiffusive;
			compute_sediment_flux(erosion_rates.mxDiffusiveErosionRate, iteration_time, mxSedimentMovementDiffusive); 

			compute_material_movement(mxSedimentMovementDiffusive,mxSedInDiffusive, mxSedOutDiffusive);

			rock = rock + mxSedInDiffusive - mxSedOutDiffusive;
			
			/* check mass conversation

			double avg_of_SedIn = mapavarage(mxSedIn);
			double avg_of_SedOut = mapavarage(mxSedIn);
			if (fabs(avg_of_SedIn-avg_of_SedOut) > 10-6) {
				std::cout << "mass conversation violated" << std::endl;
				stopCondition = false;
			}
			*/
			
                   


			/*DblRasterMx kTectIteration;
			mapattr(nSizeY,nSizeX,pixelSize,kTect*iteration_time, kTectIteration);
			DblRasterMx kTectIterationLeft;
			mapattr(nSizeY,nSizeX,pixelSize,kTectLeft*iteration_time, kTectIterationLeft);
			DblRasterMx kTectIterationRight;
			mapattr(nSizeY,nSizeX,pixelSize,kTectRight*iteration_time, kTectIterationRight);*/
			elapsedTime+=iteration_time;
			std::cout << "Iteration nr: " << iteration_nr << " elapsed time: " << elapsedTime << std::endl; 
			
			// spatially variing uplift
			/*if (elapsedTime > nextUpliftTime) {
				if (nextUpliftCol < nSizeX) {			
					++nextUpliftCol;
				}
				nextUpliftTime+=UpliftTimeInc; 
			}
			double upliftVal = iteration_time * kTect;
			for (size_t col = 0; col < nextUpliftCol; ++col) {
				for (size_t row = 0; row < nSizeY; ++row) {
					rock(row, col)+=upliftVal;
				}
			}*/
		
			// horizontal advection
			if (elapsedTime >= nextShiftTime) {
				nextShiftTime+=shiftTimeInc;
				shift_left(rock);
				shift_left(soil);
			}

			// lower Edges
			 size_t g = 0;
			 size_t h = 0;

			if (elapsedTime < 10000) {
				 for ( g = 0; g < nSizeY; g++ ){	
					 for ( h = 0; h < nSizeX; h++ ){
						 kTectMxIteration(g,h) = kTect * iteration_time;
					 }
				 }
			}
			else {
				for ( g = 0; g < nSizeY; g++ ){	
					 for ( h = 0; h < nSizeX; h++ ){
						 //kTectMxIteration(g,h) = (kTect + g * 0.001) * iteration_time;
						 kTectMxIteration(g,h) = 0;
					 }
				 }
			}
					
			EdgesLeft = EdgesLeft-kTectMxIteration;
			EdgesRight = EdgesRight-kTectMxIteration;

			    size_t j = 0;
				size_t l = 0;
				for ( j = 0; j < nSizeY; j++ ){				 
                   rock(j,0) = EdgesLeft(j,0);   
				   //rock(j,0) = rock(j,0) - kTectMxIteration(j,0);   
				   //rock(j,nSizeX-1) = EdgesRight(j,nSizeX-1);
					   for ( l = 0; l < nSizeY; l++ ){
						   if (l > EdgesLimit){
							   //rock(j,l) = rock(j,l) - kTectMxIteration(j,l);    
							   rock(j,l) = EdgesRight(j,l);
						   }
					   }
			   }
				
				// Four edges
			  /*  for ( j = 1; j < nSizeX-1; j++ ){
                       rock(nSizeY-1,j) = Edges(nSizeY-1,j);
					   rock(0,j) = Edges(0,j);	
			   }*/
			if (elapsedTime >= nextShiftTime2) {
				nextShiftTime2+=shiftTimeInc2;
				EdgesLimit=EdgesLimit+1;
			}
			

			terrain = rock + soil;
			mxFlowDepth = copyOfFlowDepth;
			accumulated_rain = copy_of_accumulated_rain;
			elapsedRainTime = copy_of_elapsedRainTime;
			redo_iteration = false;
		}

		statFile.print(elapsedTime);
		statFile.print(iteration_time);
		statFile.endl();
		if (elapsedTime > nextLogTime){
					//saveToArcgis(terrain, log_index, "terrain");
					nextLogTime+=logTimeInc;
					
			   mx1=terrain;
			   slope(mx1,mxSlope); 
			   max(mxSlope,zeroPlusABit, mxSlopeCorrected);			  
			   multiflowLDD( 1.0, mx1, terrainMLDD, false);
			   semiMultiflowLDD(mx1, mxSMLDD, false);
			   diagonal(terrainMLDD, mxDiagonal);
			   accflux(terrainMLDD,mxFluid,mxAccflux,0.0);				  
			   longestflowpathlength(mxSMLDD, mxLongest);
			   max(mxLongest, five, mxLongest);
			   log10(mxLongest, logMxLongest);
			   log10(mxAccflux, logMxAccflux);
			   elongation = logMxLongest / logMxAccflux;	   
			   
			   upstreammax( terrainMLDD, mxAccflux, mxUpstreamMax, upstreamMaxAreaPosition, false);
			   downstreammax( terrainMLDD, mxAccflux, mxDownstreamMax, downstreamMaxAreaPosition, true);
			   //upstreamtotal(mxMLDD, mxAccflux, mxUpstreamTotal, true,true);	
			   diagonal(upstreamMaxAreaPosition, upstreamMxDiagonal);
			   dAdL = (mxAccflux - mxUpstreamMax) * upstreamMxDiagonal;
			   ////max(dAdL, fifty, dAdL);
			   ////getMatrixPosItems(mxSlopeCorrected,upstreamMaxAreaPosition,upstreamSlope);
			   ////dSdL = (mxSlopeCorrected - upstreamSlope) * upstreamMxDiagonal;
			   ////sum_of_upstreamdiffs(mxMLDD, mxAccflux, sumdAdL, 0.0);
			   ////sum_of_upstreamdiffs(mxMLDD, mxSlopeCorrected, sumdSdL, 0.0);
			   ////getMatrixPosItems(sumdAdL,upstreamMaxAreaPosition,upstreamsumdAdL);  
			   //////dAdL2 = (dAdL - upstreamDAdL) * upstreamMxDiagonal; 	
			   dAdLA = dAdL / mxAccflux;
			   ////dSdLS = n * sumdSdL / mxSlopeCorrected;
			   ////nDiff = dSdLS / dAdLA;
			   ////valve = dAdLA + dSdLS;
			   ////streampower = mxAccflux^m * mxSlopeCorrected^n;
			   ////streampowervalve = streampower * valve;
			   ////getMatrixPosItems(sumdAdL,downstreamMaxAreaPosition,downstreamsumdAdL);
			   // for ( j = 0; j < nSizeX; j++ ){
						//for ( l = 1; l < nSizeY; l++ ){
						//	crest(l,j) = 0;
						//	if (mxAccflux (l,j) < 200){   
						//		crest(l,j) = 1;
						//	}
						//}
			   //}
			   //spreadLDD(mxMLDD,crest,mxShortest,0.0);
			   //spreadLDDMax( mxSMLDD, crest, mxLongest,0.0 );
			   //incision = mxLongest - mxShortest;
			   IntRasterMx terrain_pixel_types;
			   find_special_points(rock, ridge,  terrain_pixel_types);
			   mxDownstreamRatio = mxDownstreamMax / mxAccflux;
			  /* for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){
							mxDownstreamRatio(l,j) = mxDownstreamMax(l,j) / mxAccflux(l,j);
							}
			   }*/
			   int j = 0;
			   int l = 0;
			   streams.fill(0.0);
			   
			   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){									
							if ( mxDownstreamRatio(l,j) > 0.9 ){		
								streams(l,j) = 1;                  						
							}
						}
			   }			  
			   
			   path (mxSMLDD, streams, path1);
			   /*
			   for ( j = 0; j < nSizeX; j++ ){			// because of the unincluded first pixel
						for ( l = 0; l < nSizeY; l++ ){			
							if (path1(l,j) == 1 || streams(l,j) == 1){
								path1(l,j) = 1;                 
							}
						}
			   }*/
			   
			   for ( j = 0; j < nSizeX; j++ ){			
						for ( l = 0; l < nSizeY; l++ ){			
							pathsum = path1(l,j) + pathsum;							
						}
			   }
			   
			   findpits(terrain, pits); 
			   for ( j = 0; j < nSizeX; j++ ){			
						for ( l = 0; l < nSizeY; l++ ){			
							pitsum = pits(l,j) + pitsum;							
						}
			   }

			   
			    statFile2.print(pathsum);
				statFile2.print(pitsum);
				statFile2.print(EdgesLimit);
				statFile2.endl();
			

			   //saveToArcgis(mx1,log_index,"mx2");			   
					saveToArcgis(mx1,log_index,"mx1");
					saveToArcgis(mxAccflux,log_index,"accflux");
					//saveToArcgis(mxSlopeCorrected,log_index,"slope");
					//saveToAsc(sedOut, i, "sedOut", true);
					//saveToAsc(sedIn, i, "sedIn", true);
					//saveToAsc(change, i, "change",true);
					/*saveToAsc(mxLDD, i, "mxLDD");
					saveToAsc(mxMLDD, i, "mxMLDD");
					saveToAsc(mxDiagonal, i, "mxDiagonal");
					saveToAsc(streams, i, "streams");
					saveToAsc(susp, i, "susp");
					saveToAsc(mxUpstreamMax, i, "mxUpstreamMax");
					saveToAsc(crest, i, "crest",true);
					saveToAsc(mxUpstreamRatio, i, "mxUpstreamRatio");
					saveToAsc(upstreamMaxAreaPosition,i,"upstreamMaxAreaPosition");
					saveToAsc(mxDiagonal, i, "mxDiagonal");
					saveToAsc(suspSedOut, i, "suspSedOut");
					saveToAsc(suspSedIn, i, "suspSedIn");
					saveToAsc(suspSedOutLocal, i, "suspSedOutLocal");
					saveToAsc(change2, i, "change2");
					saveToAsc(mxFluid, i, "mxFluid");
					saveToAsc(mxDownstreamMax, i, "mxDownstreamMax");
					saveToAsc(mxDownstreamRatio, i, "mxDownstreamRatio",true);
					saveToAsc(points, i, "points");
					saveToAsc(points2, i, "points2");
					
					saveToAsc(catchment1, i, "catchment1");
					saveToAsc(catchment3, i, "catchment3");
					saveToAsc(streams2, i, "streams2",true);
					saveToAsc(downstreamDAdL, i, "downstreamDAdL");
					saveToAsc(dAdL2, i, "dAdL2");
					saveToAsc(focusPoint, i, "focusPoint");
					saveToAsc(focusPoint2, i, "focusPoint2");
					saveToAsc(focusPoint3, i, "focusPoint3");
					saveToAsc(catchment2, i, "catchment2");
					saveToAsc(yCoordinate, i, "yCoordinate");
					saveToAsc(xCoordinate, i, "xCoordinate");
					saveToAsc(circle2, i, "circle2");
					saveToAsc(distance, i, "distance");
					saveToAsc(distance2, i, "distance2");
					saveToAsc(line1, i, "line1");
					saveToAsc(hollow, i, "hollow");
					saveToAsc(fpAccflux, i, "fpAccflux",true);
					saveToAsc(fpSlope, i, "fpSlope",true);
					saveToAsc(fpLongest, i, "fpLongest",true);
					saveToAsc(fpYCoord, i, "fpYCoord");
					saveToAsc(fpelo, i, "fpelo",true);
					saveToAsc(fpShortest, i, "fpShortest",true);
					saveToAsc(fp2elo, i, "fp2elo",true);
					saveToAsc(fp2Shortest, i, "fp2Shortest",true);
					saveToAsc(fp2Accflux, i, "fp2Accflux",true);
					saveToAsc(fp2Slope, i, "fp2Slope",true);
					saveToAsc(fp2Longest, i, "fp2Longest",true);
					saveToAsc(fp2YCoord, i, "fp2YCoord");
					saveToAsc(fp3Accflux, i, "fp3Accflux");
					saveToAsc(fp3Slope, i, "fp3Slope");
					saveToAsc(fp3Longest, i, "fp3Longest");
					saveToAsc(dAdL, i, "dAdL",true);
					saveToAsc(upstreamDAdL, i, "upstreamDAdL");
					saveToAsc(dAdL2, i, "dAdL2");
					saveToAsc(upstreamSlope, i,"upstreamSlope");
					saveToAsc(dSdL, i, "dSdL",true);
					saveToAsc(valve, i, "valve",true);
				    saveToAsc(streampower, i, "streampower",true);
					saveToAsc(streampowervalve, i, "streampowervalve",true);
					
					saveToAsc(dSdLS, i, "dSdLS",true);
				    saveToAsc(fpdAdLA, i, "fpdAdLA",true);
					saveToAsc(fpdSdLS, i, "fpdSdLS",true);
					saveToAsc(fp2dAdLA, i, "fp2dAdLA",true);
					saveToAsc(fp2dSdLS, i, "fp2dSdLS",true);*/
					//saveToArcgis(mxUpstreamMax, log_index, "mxUpstreamMax");
					//saveToArcgis(dAdL, log_index, "dAdL");
					//saveToArcgis(dAdLA, log_index, "dAdLA");
					//saveToArcgis(mxLongest, log_index, "mxLongest");
					//saveToAsc(mxShortest, i, "mxShortest",true);
					//saveToAsc(sumdAdL, i, "sumdAdL",true);
					//saveToAsc(sumdSdL, i, "sumdSdL",true);
					//saveToArcgis(elongation, log_index, "elongation");
					//saveToAsc(incision, i, "incision",true);
					//saveToAsc(fpsumdAdL, i, "fpsumdAdL",true);
					//saveToAsc(fp2sumdAdL, i, "fp2sumdAdL",true);
					//saveToArcgis(terrain_pixel_types, log_index, "special_points_slope");
					//saveToArcgis(mxDownstreamRatio, log_index, "mxDownstreamRatio");
					//saveToArcgis(mxDownstreamMax, log_index, "mxDownstreamMax");
					//saveToArcgis(path1, log_index, "path1");
					//saveToArcgis(streams, log_index, "streams");
					//saveToArcgis(pits,log_index,"pits");
					saveToArcgis(mxSedOutDiffusive, log_index, "mxSedOutDiffusive");
					saveToArcgis(mxSedOutFluvial, log_index, "mxSedOutFluvial");
					++log_index;
		}
		iteration_nr++;
		pathsum = 0;
		pitsum = 0;
	}

	printmx(terrain);
	
//saveToArcgis(terrain, iteration_nr, "terrain");
 
	return true;
}

}