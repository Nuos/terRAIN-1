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

bool CompositSimulation::run()
{
	setOutputDirectory("d:\\terrain_output");
	// simulation settings
	setOutflowType(ofAllSides);
	//setOutflowType(ofTopAndBottomSide);
	//setOutflowType(ofLeftAndRightSide);
	
	SoilProductionType groundProductionType = gpNone;
	
	//rfCatchmentBasedEstimation rfRainfallRunoff
	RunoffProductionType runoffProductionType = rfCatchmentBasedEstimation;
	
	//stTotalRemoval stPixelPixelTransport
	SedimentTransportType sedimentTransportType = stTotalRemoval;

	double rainTime = 100;
	double max_iteration_time = 1;
	double min_iteration_time = 0.00000;
	
	erosion_rate_params erosionRateParams;
	erosionRateParams.critical_slope = M_PI/2/2;
	erosionRateParams.infinite_erosion_rate = 1;
	erosionRateParams.diffusion_exponent = 2.0;
	erosionRateParams.diffusive_const =  0.0;
	erosionRateParams.fluvial_const = 0.001;
	erosionRateParams.min_elevation_diff = 1e-7;
	erosionRateParams.runoff_exponent = 1.5;
	erosionRateParams.slope_exponent = 1.5;
	erosionRateParams.simple_diffusion = true;

	double kTect = 0.1;

	// common variables
	size_t nSizeX = 20;
	size_t nSizeY = 20;
	double pixelSize = 10;
	double rainIntensity = 1; // [length_unit/time_unit]
	double iteration_time = max_iteration_time; // [time_unit]
	double accumulated_rain = 0.0; //[length_unit] do not change
	
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

	DblRasterMx Edges;
	mapattr(nSizeY,nSizeX,pixelSize,100, Edges);

	//initial terrain and ground 
	switch (groundProductionType)
	{
		case gpNone:
		{
			mapattr(nSizeY,nSizeX,pixelSize,0.0, rock);
			// initial ground generation
			double slope = 1.0;
			double elevation = 100;
			
			DblRasterMx randomNoise;
			DblRasterMx multiplicatorSmall;
			mapattr(nSizeY,nSizeX,pixelSize,0.0, randomNoise);
			mapattr(nSizeY,nSizeX,pixelSize,0.001, multiplicatorSmall);
			mapattr(nSizeY,nSizeX,pixelSize,elevation, rock);
			uniform(randomNoise);
			rock = rock + randomNoise * multiplicatorSmall;
			mapattr(nSizeY,nSizeX,pixelSize,0.0, soil);
			saveToArcgis(randomNoise, 0, "randomNoise");
			
			/*if (!loadFromArcgis("d:\\terrain_output\\save\\terrain000010_nr1.asc",rock)) {
				std::cout << "Unable to read arc gis file" << std::endl;
				return false;
			}
			IntRasterMx terrain_pixel_types;
			find_special_points(rock, slope_point, terrain_pixel_types);
			saveToArcgis(terrain_pixel_types, 0, "special_points_slope");*/
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
	double velocity_coefficent = 1.0;

	// params of rfCatchmentBasedEstimation
	DblRasterMx mxFluid;
	mapattr(nSizeY,nSizeX,pixelSize,pixelSize*pixelSize*rainIntensity, mxFluid);


	size_t iteration_nr = 0;
	double waterOnPits = 0.0;
	double logTimeInc = 1.0;
	double nextLogTime = logTimeInc;
	int log_index = 1;
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
					// mldd of water surface
					MultiflowDMatrix  terrainMLDD;
					multiflowLDD( 1.0, terrain, terrainMLDD, false);
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

			DblRasterMx mxSedInFluvial;
			DblRasterMx mxSedOutFluvial;
			compute_material_movement(mxSedimentMovementFluvial,mxSedInFluvial, mxSedOutFluvial);

			rock = rock - mxSedOutFluvial;

			if (sedimentTransportType == stPixelPixelTransport) {
				rock = rock + mxSedInFluvial;
			}
			

			MultiflowDMatrix mxSedimentMovementDiffusive;
			compute_sediment_flux(erosion_rates.mxDiffusiveErosionRate, iteration_time, mxSedimentMovementDiffusive); 

			DblRasterMx mxSedInDiffusive;
			DblRasterMx mxSedOutDiffusive;
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
			terrain = rock + soil;

			
			DblRasterMx kTectIteration;
			mapattr(nSizeY,nSizeX,pixelSize,kTect*iteration_time, kTectIteration);
			Edges = Edges-kTectIteration;
			    size_t j = 0;
				size_t l = 0;
			   for ( j = 0; j < nSizeY; j++ ){				 
                   rock(j,0) = Edges(j,0);   
				   rock(j,nSizeX-1) = Edges(j,nSizeX-1);
					   /*for ( l = 0; l < nSizeY; l++ ){
						   if (l < 3){
							   rock(j,l) = rock(j,l) - kTectIteration(j,l);
						   }
					   }*/
			   }
							               
			    for ( j = 1; j < nSizeX-1; j++ ){
                       rock(nSizeY-1,j) = Edges(nSizeY-1,j);
					   rock(0,j) = Edges(0,j);
			   }

			mxFlowDepth = copyOfFlowDepth;
			accumulated_rain = copy_of_accumulated_rain;
			elapsedRainTime = copy_of_elapsedRainTime;
			redo_iteration = false;
		}

		elapsedTime+=iteration_time;
		std::cout << "Iteration nr: " << iteration_nr << " elapsed time: " << elapsedTime << std::endl; 
		if (elapsedTime > nextLogTime){
					saveToArcgis(terrain, log_index, "terrain");
					nextLogTime+=logTimeInc;
					++log_index;
			   //mx1=terrain;
			   //slope(mx1,mxSlope); 
			   //max(mxSlope,zeroPlusABit, mxSlopeCorrected);
			   //lddcreate(mx1, mxLDD, true);
			   //multiflowLDD( one, mx1, mxMLDD,true);
			   //semiMultiflowLDD(mx1, mxSMLDD, true);
			   //diagonal(mxMLDD, mxDiagonal);
			   //accflux(mxMLDD,mxFluid,mxAccflux,accuFluxLimitRate);					  
			   //longestflowpathlength(mxSMLDD, mxLongest);
			   //max(mxLongest, five, mxLongest);
			   //log10(mxLongest, logMxLongest);
			   //log10(mxAccflux, logMxAccflux);
			   //elongation = logMxLongest / logMxAccflux;	   
			   //upstreammax( mxMLDD, mxAccflux, mxUpstreamMax, upstreamMaxAreaPosition, true);
			   //downstreammax( mxMLDD, mxAccflux, mxDownstreamMax, downstreamMaxAreaPosition, true);
			   //upstreamtotal(mxMLDD, mxAccflux, mxUpstreamTotal, true,true);	
			   //diagonal(upstreamMaxAreaPosition, upstreamMxDiagonal);
			   //dAdL = (mxAccflux - mxUpstreamMax) * upstreamMxDiagonal;
			   ////max(dAdL, fifty, dAdL);
			   ////getMatrixPosItems(mxSlopeCorrected,upstreamMaxAreaPosition,upstreamSlope);
			   ////dSdL = (mxSlopeCorrected - upstreamSlope) * upstreamMxDiagonal;
			   ////sum_of_upstreamdiffs(mxMLDD, mxAccflux, sumdAdL, 0.0);
			   ////sum_of_upstreamdiffs(mxMLDD, mxSlopeCorrected, sumdSdL, 0.0);
			   ////getMatrixPosItems(sumdAdL,upstreamMaxAreaPosition,upstreamsumdAdL);  
			   //////dAdL2 = (dAdL - upstreamDAdL) * upstreamMxDiagonal; 	
			   ////dAdLA = m * sumdAdL / mxAccflux;
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

		}
		iteration_nr++;
	}

	printmx(terrain);
	
saveToArcgis(terrain, iteration_nr, "terrain");
 
	return true;
}

}