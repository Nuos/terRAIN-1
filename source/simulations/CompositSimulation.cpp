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


bool CompositSimulation::run()
{
	
	// simulation settings
	setOutflowType(ofAllSides);
	SoilProductionType groundProductionType = gpNone;
	
	//rfCatchmentBasedEstimation rfRainfallRunoff
	RunoffProductionType runoffProductionType = rfCatchmentBasedEstimation;
	double rainTime = 10;
	double max_iteration_time = 1;

	// common variables
	size_t nSizeX = 20;
	size_t nSizeY = 20;
	double pixelSize = 10;
	double rainIntensity = 1; // [length_unit/time_unit]
	double accumulated_rain = 0.0; //[length_unit]
	double iteration_time = max_iteration_time; // [time_unit]
	DblRasterMx mxRain;
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxRain);
	DblRasterMx mxFlowDepth;
	mapattr(nSizeY,nSizeX,pixelSize,0.0, mxFlowDepth);
	double elapsedTime = iteration_time;
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
			double slope = 1.0;
			double elevation = 100;

			mapattr(nSizeY,nSizeX,pixelSize,0.0, soil);
			for (size_t i = 0; i < nSizeX; i++){
				for (size_t j = 0; j < nSizeY; j++){
					rock(i,j)=elevation + (i+j)*slope;	
				}
			}
			//rock(0,0) = 0;
			break;
		}
		default:
			std::cout << "Invalid ground type production type: " << groundProductionType;
			return false;
	}
	DblRasterMx terrain;
    terrain = rock + soil;

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
	while (stopCondition) 
	{
		terrain = rock + soil;
		MultiflowDMatrix runoff_distribution;
		switch (runoffProductionType)
		{
			case rfRainfallRunoff:
			{
				double rain_time_within_iteration = min(iteration_time, rainTime - elapsedRainTime);
				double rain = rain_time_within_iteration * rainIntensity;
				bool isRain = false;
				if (rain > 0.0) {
					for (size_t i = 0; i < nSizeY; i++){
						for (size_t j = 0; j < nSizeX; j++){
							mxRain(i,j)=rain;	
						}
					}
					mxFlowDepth = mxFlowDepth + mxRain;
					accumulated_rain += rain*pixelSize*pixelSize*nSizeX*nSizeY;
					elapsedRainTime+=rain_time_within_iteration; 
					isRain = true;
				}
				DblRasterMx waterSurface;
				waterSurface = terrain + mxFlowDepth;

				MultiflowDMatrix  mxWaterSurfaceAngles;
				multiflowAngles(waterSurface, mxWaterSurfaceAngles, true);

				// mldd of water surface
				MultiflowDMatrix  waterMLDD;
				multiflowLDD( 1.0, waterSurface, waterMLDD, true);

				MultiflowDMatrix flux_distribution;
				compute_flux_distribution(waterMLDD, mxFlowDepth, flux_distribution);

				MultiflowDMatrix velocity_mldd;
				double time_interval = compute_velocity_mldd(flux_distribution, mxWaterSurfaceAngles, velocity_coefficent,  velocity_mldd);
				time_interval = ::min(time_interval, max_iteration_time);
				iteration_time = time_interval;
				MultiflowDMatrix mxOutflowFlux;
				compute_outflow_flux_mldd( velocity_mldd, flux_distribution, time_interval,mxOutflowFlux);

				DblRasterMx mxInFlow;
				DblRasterMx mxOutFlow;
				compute_material_movement(mxFlowDepth,mxOutflowFlux,mxInFlow, mxOutFlow);
				mxFlowDepth = mxFlowDepth - mxOutFlow + mxInFlow;

				DblRasterMx::iterator iOutFlowPITs = outflowPITs.begin(), endOutFlowPITS = outflowPITs.end();
				DblRasterMx::iterator iFlowDepth = mxFlowDepth.begin();
				
				for (; iOutFlowPITs!=endOutFlowPITS; ++iOutFlowPITs, ++iFlowDepth) {
					if (*iOutFlowPITs > 0.0) {
						waterOnPits+= *iFlowDepth * pixelSize * pixelSize;
						*iFlowDepth = 0.0;
					}
				}
				compute_runoff_distribution( velocity_mldd,mxFlowDepth,runoff_distribution);
				// check only if there is no more rainfall
				if (!isRain) {
					std::cout << "volume of fluid in outflow position: = " << waterOnPits << std::endl;
					if ( fabs(waterOnPits - accumulated_rain) < DoubleUtil::sDeltaD3) {
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
				multiflowLDD( 1.0, terrain, terrainMLDD, true);
				DblRasterMx mxAccflux;
				accflux(terrainMLDD,mxFluid,mxAccflux,0.0);
				compute_flux_distribution(terrainMLDD, mxAccflux, runoff_distribution);

				if ( elapsedTime >= rainTime) {
					stopCondition = false;
				}

				if ((iteration_nr % 10) ==0) {
					printmx(runoff_distribution);
					printmx(mxAccflux);

				}
			}
			break;
		}




		elapsedTime+=iteration_time;
		std::cout << "Iteration nr: " << iteration_nr << " elapsed time: " << elapsedTime << std::endl; 
		iteration_nr++;
	}



	return true;
}

}