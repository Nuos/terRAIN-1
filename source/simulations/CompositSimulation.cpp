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
#include "ChannelHead.h"

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

double compare_prediction_matrix( IntRasterMx & channelHeads, IntRasterMx & predictionMx)
{
	int nr_of_predictions = 0;
	int nr_of_true_predictions = 0;
	size_t nSizeX = predictionMx.getColNr();
	size_t nSizeY = predictionMx.getRowNr();
	for ( int j = 0; j < nSizeX; j++ ){
			for ( int l = 0; l < nSizeY; l++ ){
				int predicted_id = predictionMx(j,l);
				int id = channelHeads(j,l);
				if ( id > 0) {
					++nr_of_predictions;
					if ( predicted_id == id) {
						++nr_of_true_predictions;
					}
				}
			}
	}

	if (nr_of_predictions > 0)
		return static_cast<double>(nr_of_true_predictions)/nr_of_predictions;

	return 0.0;
}
bool CompositSimulation::run()
{
	setOutputDirectory("c:\\terrain_output");
	//setOutputDirectory("e:\\Solyom\\run1");
	//setOutputDirectory("i:\\Solyom\\run1");
	// simulation settings
	//setOutflowType(ofAllSides);	
	//setOutflowType(ofTopAndBottomSide);

	//ofRightSideAndTopLeft
	//ofRightSideAndLeftMiddle
	
	setOutflowType(ofLeftAndRightSide);
	//(ofRightSide);
	//setOutflowType(ofTopSide);
	//setOutflowType(ofRightSideAndTopLeft);
	
	SoilProductionType groundProductionType = gpNone;
	
	//rfCatchmentBasedEstimation rfRainfallRunoff
	RunoffProductionType runoffProductionType = rfCatchmentBasedEstimation;
	
	//stTotalRemoval stPixelPixelTransport
	SedimentTransportType sedimentTransportType = stPixelPixelTransport;
	
	double rainTime = 100;
	double logTimeInc = 1;
	double max_iteration_time = 1;
	double min_iteration_time = 0.001;
	
	erosion_rate_params erosionRateParams;
	erosionRateParams.critical_slope = M_PI/2/2;
	erosionRateParams.infinite_erosion_rate = 1;
	erosionRateParams.diffusion_exponent = 2.0;
	erosionRateParams.min_elevation_diff = 1e-7;
	erosionRateParams.runoff_exponent = 1.5;
	erosionRateParams.slope_exponent = 1.5;
	erosionRateParams.simple_diffusion = true;

	double kTect = 0.0;
	double kTectLeft = 0.1;
	double kTectRight = 0.1;
	double kHor = 0.0;
	double kTectSpread = 0.1;

	// common variables
	size_t nSizeX = 20;
	size_t nSizeY = 20;
	double EdgesLimit = 62;  //2;
	double pixelSize = 10;
	double rainIntensity = 1; // [length_unit/time_unit]
	double iteration_time = max_iteration_time; // [time_unit]
	double accumulated_rain = 0.0; //[length_unit] do not change
	double pathsum = 0;
	double pitsum = 0;
	double mx1sum = 0;
	double lakeLevel = 100;
	double rockLeftMax = -9999;
	
	DblRasterMx pixelSizeMatrix;
	DblRasterMx mx1;
	DblRasterMx mxSlope;
	DblRasterMx mxSlopeCorrected;
	DblRasterMx zeroPlusABit;
	DblRasterMx five;
	DblRasterMx tst;
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
	DblRasterMx proOutflowPoints;
	DblRasterMx proCatchment;
	DblRasterMx retroOutflowPoints;
	DblRasterMx retroCatchment;
	DblRasterMx mxSedFluvialDifference;
	DblRasterMx mxSedFluvialDifferenceRatio;
	DblRasterMx mask;
	DblRasterMx channelHeads;
	DblRasterMx pitCatchments;

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
	mapattr(nSizeY,nSizeX,pixelSize,1000, tst);
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
	mapattr(nSizeY,nSizeX,pixelSize,0.0, bedloadRatio);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, kTectMxIteration);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, proOutflowPoints);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, proCatchment);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, retroOutflowPoints);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, retroCatchment);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxSedFluvialDifference);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxSedFluvialDifferenceRatio);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mask);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, channelHeads);
	mapattr(nSizeY,nSizeX,pixelSize,0.0, pitCatchments);
	
	mapattr(nSizeY,nSizeX,pixelSize, 0.0, erosionRateParams.diffusive_const);
	mapattr(nSizeY,nSizeX,pixelSize,0.001, erosionRateParams.fluvial_const); //0.00001
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

	int nInitVal = 0;
    IntRasterMx predictionAccFluxMx(nSizeX,nSizeY, pixelSize,origoBottomLeft, nInitVal);
	IntRasterMx predictionSedOutFluvialMx(nSizeX,nSizeY, pixelSize,origoBottomLeft, nInitVal);
	IntRasterMx predictiondAdLMx(nSizeX,nSizeY, pixelSize,origoBottomLeft, nInitVal);
	IntRasterMx predictiondAdLAMx(nSizeX,nSizeY, pixelSize,origoBottomLeft, nInitVal);

	int lastChannelHeadID = 0;
	int prevChannelHead_nr = 0;
	IntRasterMx lastChannelHeadIdMx(nSizeX,nSizeY, pixelSize,origoBottomLeft, nInitVal);


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
			mapattr(nSizeY,nSizeX,pixelSize,0.1, multiplicatorSmall);
			mapattr(nSizeY,nSizeX,pixelSize,elevation, rock);
			uniform(randomNoise);
			
			mapattr(nSizeY,nSizeX,pixelSize,0.0, soil);
			//saveToArcgis(randomNoise, 0, "randomNoise");

			xcoordinate(rock, xCoord);
			ycoordinate(rock, yCoord);
			int j = 0;
			int l = 0;
			//for ( j = 0; j < nSizeX; j++ ){		
			//			for ( l = 0; l < nSizeY; l++ ){		
			//				rock(l,j) = 100 + 2 - xCoord(l,j) * 0.01;
			//				//if (j < 1){
			//				//	rock(l,j) = 101; // - xCoord(l,j) * 0.02;   
			//				////	//EdgesRight(l,j) = 100 + 0.04 - xCoord(l,j) * 0.0001;       
			//				//} else {
			//				//	rock(l,j) = 80; // - xCoord(l,j) * 0.001;
			//				////	//rock(l,j) = 100 + 0.2 - xCoord(l,j) * 0.001;
			//				//}
			//			}
			//   }
			rock = rock + randomNoise * multiplicatorSmall;
			
													
			
			//if (!loadFromArcgis("e:\\Solyom\\run1\\mx1000101b.asc",rock)) {
			/*if (!loadFromArcgis("c:\\terrain_output\\mx1000006.asc",rock)) {
				std::cout << "Unable to read arc gis file" << std::endl;
				return false;
			}*/
			 size_t g = 0;
			 size_t h = 0;
			for ( g = 0; g < nSizeY; g++ ){	
					 //for ( h = 0; h < nSizeX; h++ ){
						 retroOutflowPoints(g,0) = 1;
					 //}
				 }
			for ( g = 0; g < nSizeY; g++ ){	
					 //for ( h = 0; h < nSizeX; h++ ){
						 proOutflowPoints(g,nSizeX - 1) = 1;
					 //}
				 }

			//findpits(rock, pits);
			//saveToArcgis(pits, 0, "pits");

			IntRasterMx terrain_pixel_types;
			find_special_points(rock, channel, terrain_pixel_types);
			saveToArcgis(terrain_pixel_types, 0, "special_points_slope");
			break;
			
		}
		default:
			std::cout << "Invalid ground production type: " << groundProductionType;
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
	int j = 0;
	int l = 0;
	for ( j = 0; j < nSizeX; j++ ){		
						for ( l = 0; l < nSizeY; l++ ){			
							if (j < 1){
								mxFluid(l,j) =	mxFluid(l,j) * 1;		//(20 - xCoord(l,j)*0.1) * mxFluid(l,j); 
							//} else {
							//	rock(l,j) = 96 - xCoord(l,j) * 0.001;
							//	//rock(l,j) = 100 + 0.2 - xCoord(l,j) * 0.001;
							}
						}
			   }
							//mxFluid(15,0) = mxFluid(15,0)*1;
							//mxFluid(6,0) = mxFluid(6,0)*100;
							//mxFluid(13,0) = mxFluid(13,0)*100;
							//mxFluid(24,0) = mxFluid(24,0)*1;

							/*mxFluid(28,0) = mxFluid(28,0)*10;
							mxFluid(29,0) = mxFluid(29,0)*10;
							mxFluid(30,0) = mxFluid(30,0)*10;
							mxFluid(31,0) = mxFluid(31,0)*10;*/



	size_t iteration_nr = 0;
	double waterOnPits = 0.0;
	//double logTimeInc = 10;
	double nextLogTime = 0.0;
	int log_index = 1;
	

	double UpliftTimeInc = kTectSpread > 0 ? pixelSize/kTectSpread : DoubleUtil::getMAXValue();
	double nextUpliftTime = UpliftTimeInc;
	size_t nextUpliftCol = 0;

	double shiftTimeInc = kHor > 0 ? pixelSize/kHor : DoubleUtil::getMAXValue();
	double nextShiftTime = shiftTimeInc;
	double shiftTimeInc2 = kTectSpread > 0 ? pixelSize/kTectSpread : DoubleUtil::getMAXValue();
	double nextShiftTime2 = shiftTimeInc2;	
	
	StatFile statFile("statfile.txt",15);
	statFile.print("elapsed time");
	statFile.print("iteration time");
	statFile.endl();
	
	StatFile statFile2("statfile2.txt",15);
	statFile2.print("pathsum");
	statFile2.print("pitsum");
	statFile2.print("EdgesLimit");
	statFile2.print("mx1sum");
	statFile2.print("lake level");
	statFile2.print("rockLeftMax");
	statFile2.endl();

	StatFile statChannelHeads("statfile_channelheads.txt",25);
	statChannelHeads.print("nr_of_channel_heads");
	statChannelHeads.print("rate_of_captures_prev_timestep");
	statChannelHeads.print("rate_of_moved_channel_heads");
	statChannelHeads.print("rate_of_matching_prediction_accflux");
	statChannelHeads.print("rate_of_matching_prediction_sedoutfluvial");
	statChannelHeads.print("rate_of_matching_prediction_dAdL");
	statChannelHeads.print("rate_of_matching_prediction_dAdLA");
	
	statChannelHeads.endl();

	
	ChannelHeadTracker channelHeadTracker(nSizeX, nSizeY, pixelSize);
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
					semiMultiflowLDD(terrain, mxSMLDD, false);
					DblRasterMx mxAccflux;
					accflux(terrainMLDD,mxFluid,mxAccflux,0.0);
					//accflux(mxSMLDD,mxFluid,mxAccflux,0.0);
					compute_flux_distribution(terrainMLDD, mxAccflux, runoff_distribution);
					
					if ( elapsedTime >= rainTime) {
						stopCondition = false;
					}
				}
				break;
			}
							
			catchment(mxSMLDD, retroOutflowPoints, retroCatchment);
			catchment(mxSMLDD, proOutflowPoints, proCatchment);
			 size_t g = 0;
			 size_t h = 0;
			for ( g = 0; g < nSizeY; g++ ){	
					 for ( h = 0; h < nSizeX; h++ ){
						 if (retroCatchment(g,h) == 1) {
							 bedloadRatio(g,h) = 1.0;
						 } else {
							 bedloadRatio(g,h) = 1.0;
						 }
					 }
			 }
			//lakeLevel = 100 - (elapsedTime * 0.01); // + sin(elapsedTime/10);
			//for ( g = 0; g < nSizeY; g++ ){	
			//		 for ( h = 0; h < nSizeX; h++ ){
			//			 if (rock(g,h) < lakeLevel && rock(g,h) >= (lakeLevel - 5)) {
			//				 erosionRateParams.diffusive_const(g,h) = 0.001;
			//				 erosionRateParams.fluvial_const(g,h) = 0.0;
			//				 /*if (rock(g,h) < 98) {
			//					 erosionRateParams.diffusive_const(g,h) = 0.0000001;
			//					 erosionRateParams.fluvial_const(g,h) = 0.0;
			//				 }*/
			//			 } if (rock(g,h) < (lakeLevel - 2)) {
			//				 erosionRateParams.diffusive_const(g,h) = 0.00001;
			//				 erosionRateParams.fluvial_const(g,h) = 0.0;
			//			 } else {
			//				erosionRateParams.diffusive_const(g,h) = 0.00;
			//				erosionRateParams.fluvial_const(g,h) = 0.00001;
			//			 }
			//		 }
			// }

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

			//mxSedOutFluvial(nSizeY-1,0) = tst(nSizeY-1,0);
			//mxSedOutFluvial(0,0) = tst(0,0);

			//for ( j = 0; j < nSizeY; j++ ){			  
			//		   for ( l = 0; l < nSizeX; l++ ){
			//			   if (l == 0){	
			//				 //if (mxSedOutFluvial(j,l) > tst(j,l)){
			//					 mxSedOutFluvial(j,l) == tst(j,l);
			//				 //}
			//			   }
			//		   }					   
			//   }

			rock = rock - mxSedOutFluvial;

			if (sedimentTransportType == stPixelPixelTransport) {
				rock = rock + (mxSedInFluvial * bedloadRatio);
			}
			

			MultiflowDMatrix mxSedimentMovementDiffusive;
			compute_sediment_flux(erosion_rates.mxDiffusiveErosionRate, iteration_time, mxSedimentMovementDiffusive); 

			compute_material_movement(mxSedimentMovementDiffusive,mxSedInDiffusive, mxSedOutDiffusive);

			rock = rock + mxSedInDiffusive - mxSedOutDiffusive;

			mxSedFluvialDifference = mxSedOutFluvial - mxSedInFluvial;

			mxSedFluvialDifferenceRatio = (mxSedFluvialDifference / (mxSedOutFluvial*tst))*tst;
			
			/* check mass conservation*/

			double avg_of_SedIn = mapavarage(mxSedInFluvial);
			double avg_of_SedOut = mapavarage(mxSedOutFluvial);
			if (fabs(avg_of_SedIn-avg_of_SedOut) > 10-6) {
				std::cout << "mass conversation violated" << std::endl;
				stopCondition = false;
			}
			/**/
			
                   


			DblRasterMx kTectIteration;
			mapattr(nSizeY,nSizeX,pixelSize,kTect*iteration_time, kTectIteration);
			DblRasterMx kTectIterationLeft;
			mapattr(nSizeY,nSizeX,pixelSize,kTectLeft*iteration_time, kTectIterationLeft);
			DblRasterMx kTectIterationRight;
			mapattr(nSizeY,nSizeX,pixelSize,kTectRight*iteration_time, kTectIterationRight);		
				
			for ( j = 0; j < nSizeX; j++ ){		
						for ( l = 0; l < nSizeY; l++ ){			
							if (j < 1){
							////if (mxFluid(l,j) == 10000){
							//	//kTectIteration(l,j) =	(xCoord(l,j)*0.01) * kTectIteration(l,j); 
								kTectIteration(l,j) =	0; 
							//	//kTectIterationLeft(l,j) = kTectIterationLeft(l,j) * 10;
							} if (j > 10){
							//	//kTectIteration(l,j) = (2 - (xCoord(l,j)*0.01)) * kTectIteration(l,j);	
								kTectIteration(l,j) =	0; 
							//} else {
								//kTectIteration(l,j) = (1-abs(1-(xCoord(l,j)*0.01))/1.1) * kTectIteration(l,j);	
								//kTectIteration(l,j) = (2-abs(2-(xCoord(l,j)*0.01))/1.1) * kTectIteration(l,j);	// /0.6, 1.1
							}
						}
			   }

							//kTectIterationLeft(6,0) = kTectIteration(6,5)*1;
							//kTectIterationLeft(15,0) = kTectIteration(15,0)*1;
							//kTectIterationLeft(24,0) = kTectIteration(24,0)*1;
							//kTectIterationLeft(13,0) = kTectIteration(13,5)*1;

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
			 //size_t g = 0;
			 //size_t h = 0;

			if (elapsedTime < 1) {
				//kTectMxIteration.fill( kTect * iteration_time * 1);				
				for ( g = 0; g < nSizeY; g++ ){	
					 for ( h = 0; h < nSizeX; h++ ){
					 //kTectMxIteration(g,h) = (kTect + g * 0.01) * iteration_time;
						kTectIteration(g,h) = kTect * iteration_time * 1;
						//kHor = 0.0;
					 }
				 }
			}
			else {
				//kTectMxIteration.fill(0.0);
				for ( g = 0; g < nSizeY; g++ ){	
					 for ( h = 0; h < nSizeX; h++ ){
						 //kTectMxIteration(g,h) = (kTect - g * 0.001) * iteration_time;
						 kTectIteration(g,h) = kTect * iteration_time * 1;
						 //kHor = 1;
					 }
				 }
			}
			
			rockLeftMax = -9999;
			EdgesLeft = EdgesLeft - kTectIterationLeft;
			EdgesRight = EdgesRight - kTectIterationRight;
				/*for ( j = 0; j < nSizeY; j++ ){			  
					   for ( l = 0; l < nSizeX; l++ ){
						   if (l == 0){	
							 if (rock(j,l) > rockLeftMax){
								 rockLeftMax == rock(j,l);
							 }
						   }
					   }					   
			   }*/
			/*	rockLeftMax=rock(0,0);
			for ( j = 0; j < nSizeY; j++ ){			  
					   for ( l = 0; l < nSizeX; l++ ){
						   EdgesRight(j,l) = rockLeftMax + 2;
					   }					   
			   }*/
			

			    size_t j = 0;
				size_t l = 0;
				for ( j = 0; j < nSizeY; j++ ){		                  
				   //rock(j,0) = rock(j,0) - kTectMxIteration(j,0); 
					//rock(j,0) = rock(j,0) + kTectMxIteration(j,0); 
				   //rock(j,nSizeX-1) = rock(j,nSizeX-1) - kTectMxIteration(j,nSizeX-1); 
					   for ( l = 0; l < nSizeX; l++ ){
						   if (l == 0){		
							   rock(j,l) = EdgesLeft(j,l);
							  //rock(j,l) = rock(j,l) + kTectIterationLeft(j,l);  // * tst(j,l); // * ((EdgesLimit - l)/ EdgesLimit); 
						   //} if (l < EdgesLimit && l > 0){		
							    } if (l < nSizeX-1 && l > 0){		
							//   if (l < 1){					
							  //rock(j,l) = rock(j,l) + kTectIterationLeft(j,l); // * tst(j,l); // * ((EdgesLimit - l)/ EdgesLimit); 
							   rock(j,l) = rock(j,l) + kTectIteration(j,l); // * tst(j,l);   
						  //} if (l >= EdgesLimit){		
						   } if (l == nSizeX-1){
							   // } else {						     
							  //rock(j,l) = rock(j,l) - kTectIterationRight(j,l); // * tst(j,l);    
							  rock(j,l) = EdgesRight(j,l);
						   }
					   }
					   //rock(j,0) = EdgesLeft(j,0);   
					   //rock(j,nSizeX-1) = EdgesRight(j,nSizeX-1);
					   //bedloadRatio(l,EdgesLimit-1) = 0;
					   //bedloadRatio(l,EdgesLimit-2) = 0;
					   //bedloadRatio(l,EdgesLimit-3) = 0;
			   }
				//rock(14,0) = rock(14,0) + kTectMxIteration(14,0) * 10; 
				// Four edges
			  /*  for ( j = 1; j < nSizeX-1; j++ ){
                       rock(nSizeY-1,j) = Edges(nSizeY-1,j);
					   rock(0,j) = Edges(0,j);	
			   }*/

							/*rock(5,2) = 200;
							rock(6,2) = 200;
							rock(7,2) = 200;
							rock(8,2) = 200;
							rock(9,2) = 200;
							rock(10,2) = 200;
							rock(11,2) = 200;
							rock(12,2) = 200;
							rock(13,2) = 200;
							rock(14,2) = 200;
							rock(15,2) = 200;														
							rock(16,2) = 200;
							rock(17,2) = 200;

							rock(23,2) = 200;
							rock(24,2) = 200;
							rock(25,2) = 200;
							rock(26,2) = 200;
							rock(27,2) = 200;
							rock(28,2) = 200;
							rock(29,2) = 200;
							rock(30,2) = 200;
							rock(31,2) = 200;
							rock(32,2) = 200;
							rock(33,2) = 200;
							rock(34,2) = 200;		*/					
			
			if (elapsedTime >= nextShiftTime2 && EdgesLimit < nSizeX-2) {
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
			   //IntRasterMx terrain_pixel_types;
			   //find_special_points(rock, ridge,  terrain_pixel_types);
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
			   findpits(terrain, pits); 
			   catchment(mxSMLDD, pits, pitCatchments);
			   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){									
							if ( pitCatchments(l,j) == 1 ){		
								path1(l,j) = 0;                  						
							}
						}
			   }			  
			   findChannelHeads(mxSMLDD, path1, channelHeads);
			   int nr_of_captures = channelHeadTracker.track(channelHeads, path1, elapsedTime); 
			   //statChannelHeads
			   IntRasterMx & channelHeadMx = channelHeadTracker.channelHeads();
			   IntRasterMx movedChannelHeads = channelHeadMx;

			   ChannelHeadMap & channelHeadMap = channelHeadTracker.channelHeadMap();
			   // build statistics:

			   // nr of moved channel heads / nr of channel heads / nr of new channel heads
			   int nr_of_new_channel_heads = 0;
			   int nr_of_channel_heads = 0;
			   int nr_of_channel_heads_prev = 0;
			   int nr_of_not_moved_channel_heads = 0;
			   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){
							int id_prev = lastChannelHeadIdMx(j,l); 
							int id = channelHeadMx(j,l);

							if (id_prev > 0)
								++nr_of_channel_heads_prev;

							if (id > 0)
								++nr_of_channel_heads;

							if (id > 0 && id==id_prev) {
								++nr_of_not_moved_channel_heads;
								// remove not moved channel heads
								movedChannelHeads(j,l) = 0;
							}
							
							if (id > lastChannelHeadID) {
								++nr_of_new_channel_heads;
								// remove new channel heads
								movedChannelHeads(j,l) = 0;
							}
						}
			   }

			   double rate_of_moved_channel_heads = 1.0;
			   if ( nr_of_channel_heads_prev > 0 )
					rate_of_moved_channel_heads = static_cast<double>(nr_of_channel_heads_prev - nr_of_not_moved_channel_heads) / nr_of_channel_heads_prev;


			   statChannelHeads.print( nr_of_channel_heads ); 
			   double rate_of_captures = prevChannelHead_nr > 0 ? static_cast<double>(nr_of_captures) / prevChannelHead_nr : 0.0;
			   statChannelHeads.print( rate_of_captures );
			   statChannelHeads.print( rate_of_moved_channel_heads );

			   // compare channelHeadMx and predictionMx
			   double rate_of_true_predictions_accflux = compare_prediction_matrix(movedChannelHeads, predictionAccFluxMx); 
			   double rate_of_true_predictions_SedOutFluvial = compare_prediction_matrix(movedChannelHeads, predictionSedOutFluvialMx);
			   double rate_of_true_predictions_dAdL = compare_prediction_matrix(movedChannelHeads, predictiondAdLMx);
			   double rate_of_true_predictions_dAdLA = compare_prediction_matrix(movedChannelHeads, predictiondAdLAMx);
			   
			   statChannelHeads.print( rate_of_true_predictions_accflux );
			   statChannelHeads.print( rate_of_true_predictions_SedOutFluvial ); 
			   statChannelHeads.print( rate_of_true_predictions_dAdL );
			   statChannelHeads.print( rate_of_true_predictions_dAdLA );
			   statChannelHeads.endl();
			   // new prediction:
			   predictionAccFluxMx.fill(0);

			   prevChannelHead_nr = nr_of_channel_heads;
			   for ( j = 0; j < nSizeX; j++ ){
						for ( l = 0; l < nSizeY; l++ ){
							int id = channelHeadMx(j,l); 
							if ( id > 0 ){		
								//ChannelHead & currentChannelHead = channelHeadMap[id];
								//pos : j,l
								// prediction:
								//predictionMx

								//mxSlope
								//DblRasterMx::iterator it = mxSlope.getIteratorAt(j,l);
								DblRasterMx::iterator itAccflux = mxAccflux.getIteratorAt(j,l);
								DblRasterMx::iterator itSedOutFluvial = mxSedOutFluvial.getIteratorAt(j,l);
								DblRasterMx::iterator itdAdL = dAdL.getIteratorAt(j,l);
								DblRasterMx::iterator itdAdLA = dAdLA.getIteratorAt(j,l);
								
								//* begin prediction based on mxAccflux
								unsigned char max_val_cc = 5;
								double max_val = -1.0;
								for (unsigned char cc = 1; cc < 10; ++cc) {
									if (cc==5 || !itAccflux.isValidItemByChainCode(cc)) {
										continue;									
									}
									double value_acc = itAccflux.chain_code(cc);			// chain code: 1...9
									if ( value_acc > max_val) {
										max_val = value_acc;
										max_val_cc = cc;
									}
								}

								DblRasterMx::iterator::dCoords delta = itAccflux.getChainCodeDelta(max_val_cc);	// path to neighbour
								int new_row = j + delta._nDRow;
								int new_col = l + delta._nDCol;
								predictionAccFluxMx(new_row, new_col) = id;
								//* end prediction based on mxAccflux

								//* begin prediction based on mxSedOutFluvial
								max_val_cc = 5;
								max_val = -1.0;
								for (unsigned char cc = 1; cc < 10; ++cc) {
									if (cc==5 || !itSedOutFluvial.isValidItemByChainCode(cc)) {
										continue;									
									}
									double value_sedOutFluvial = itSedOutFluvial.chain_code(cc);			// chain code: 1...9
									if ( value_sedOutFluvial > max_val) {
										max_val = value_sedOutFluvial;
										max_val_cc = cc;
									}
								}

								delta = itSedOutFluvial.getChainCodeDelta(max_val_cc);	// path to neighbour
								new_row = j + delta._nDRow;
								new_col = l + delta._nDCol;
								predictionSedOutFluvialMx(new_row, new_col) = id;
								//* end prediction based on mxSedOutFluvial

								//* begin prediction based on dAdL
								max_val_cc = 5;
								max_val = -1.0;
								for (unsigned char cc = 1; cc < 10; ++cc) {
									if (cc==5 || !itdAdL.isValidItemByChainCode(cc)) {
										continue;									
									}
									double value_dAdL = itdAdL.chain_code(cc);			// chain code: 1...9
									if ( value_dAdL > max_val) {
										max_val = value_dAdL;
										max_val_cc = cc;
									}
								}

								delta = itdAdL.getChainCodeDelta(max_val_cc);	// path to neighbour
								new_row = j + delta._nDRow;
								new_col = l + delta._nDCol;
								predictiondAdLMx(new_row, new_col) = id;
								//* end prediction based on dAdL

								//* begin prediction based on dAdLA
								max_val_cc = 5;
								max_val = -1.0;
								for (unsigned char cc = 1; cc < 10; ++cc) {
									if (cc==5 || !itdAdLA.isValidItemByChainCode(cc)) {
										continue;									
									}
									double value_dAdLA = itdAdLA.chain_code(cc);			// chain code: 1...9
									if ( value_dAdLA > max_val) {
										max_val = value_dAdLA;
										max_val_cc = cc;
									}
								}

								delta = itdAdLA.getChainCodeDelta(max_val_cc);	// path to neighbour
								new_row = j + delta._nDRow;
								new_col = l + delta._nDCol;
								predictiondAdLAMx(new_row, new_col) = id;
								//* end prediction based on dAdLA

							}
						}
			   }

			   lastChannelHeadIdMx = channelHeadMx;
			   lastChannelHeadID = channelHeadTracker.lastID();

			   for ( j = 0; j < nSizeX; j++ ){			
						for ( l = 0; l < nSizeY; l++ ){			
							pathsum = path1(l,j) + pathsum;							
						}
			   }
			   
			   
			   for ( j = 0; j < nSizeX; j++ ){			
						for ( l = 0; l < nSizeY; l++ ){			
							pitsum = pits(l,j) + pitsum;							
						}
			   }
			   for ( j = 0; j < nSizeX; j++ ){			
						for ( l = 0; l < nSizeY; l++ ){			
							mx1sum = mx1(l,j) + mx1sum;							
						}
			   }

			   
			    statFile2.print(pathsum);
				statFile2.print(pitsum);
				statFile2.print(EdgesLimit);
				statFile2.print(mx1sum);
				statFile2.print(lakeLevel);
				statFile2.print(rockLeftMax);
				statFile2.endl();
			

			   //saveToArcgis(mx1,log_index,"mx2");			   
					saveToArcgis(mx1,log_index,"mx1");
					saveToArcgis(mxAccflux,log_index,"accflux");
					//saveToArcgis(kTectIteration,log_index,"kTectIteration");
					//saveToArcgis(mxSlopeCorrected,log_index,"slope");
					//saveToArcgis(mxFluid, log_index, "mxFluid");
					//saveToArcgis(proOutflowPoints, log_index, "pro");
					//saveToArcgis(proCatchment, log_index, "proc");
					//saveToArcgis(retroOutflowPoints, log_index, "retro");
					//saveToArcgis(retroCatchment, log_index, "retroc");
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
					saveToArcgis(channelHeads, log_index, "channelHeads");
					saveToArcgis(channelHeadMx, log_index, "channelHeadMx");
					
					//saveToArcgis(streams, log_index, "streams");
					saveToArcgis(pits,log_index,"pits");
					saveToArcgis(path1,log_index,"path1");
					saveToArcgis(pitCatchments,log_index,"pitCatchments");
					saveToArcgis(mxSedInFluvial, log_index, "mxSedInFluvial");
					saveToArcgis(mxSedOutFluvial, log_index, "mxSedOutFluvial");
					saveToArcgis(mxSedFluvialDifference, log_index, "mxSedFluvialDifference");
					saveToArcgis(mxSedFluvialDifferenceRatio, log_index, "mxSedFluvialDifferenceRatio");
					++log_index;
		}
		iteration_nr++;
		pathsum = 0;
		pitsum = 0;
		mx1sum = 0;
	}

	printmx(terrain);
	
//saveToArcgis(terrain, iteration_nr, "terrain");
 
	return true;
}

}