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

SimpleRunoff::SimpleRunoff(ostream & os, ParamHandler & params):
	Simulation(os,params)
{}

bool SimpleRunoff::run()
{
	setOutflowType(ofAllSides);

	size_t nSizeX = 20;
	size_t nSizeY = 20;
	double pixelSize = 10;
	double slope = 1.0;
	double elevation = 100;
	StatFile statFile("statfile");
	
	DblRasterMx terrain;
	// init terrain: let an 1 pixel width edge at the boundary of the terrain
	// to collect the rain
	mapattr(nSizeY+2,nSizeX+2,pixelSize,0.0, terrain);
	for (size_t i = 1; i < nSizeX; i++){
		for (size_t j = 1; j <= nSizeY; j++){
			terrain(i,j)=elevation + (i+j)*slope;	
		}
	}

	IntRasterMx terrain_pixel_types;
	find_special_points(terrain, ridge | peak | col | ditch, terrain_pixel_types);
	MultiflowDMatrix  mxMLDD;
	multiflowLDD( 1.0, terrain, mxMLDD, true);

	MultiflowDMatrix  mxMultiflowAngles;
	multiflowAngles(terrain, mxMultiflowAngles,true);

	//legyyen rain intesity
	double rainSpeed = pixelSize/10000; // [length_unit/time_unit]
	double amount_of_rain = pixelSize/100; // [length_unit]
	double accumulated_rain = 0.0; //[length_unit]
	double iteration_time = 1.0; // [time_unit]
	
	DblRasterMx mxRain;
	mapattr(nSizeY+2,nSizeX+2,pixelSize,0.0, mxRain);

	// legyen mxFlowDepth
	DblRasterMx mxFluid;
	mapattr(nSizeY+2,nSizeX+2,pixelSize,0.0, mxFluid);

	double velocity_coefficent = pixelSize*pixelSize;

	double rain_full_volume = amount_of_rain * pixelSize * pixelSize * nSizeX * nSizeY;
	while (true) {
		
		double rain = iteration_time * rainSpeed;
		rain = min(rain, amount_of_rain - accumulated_rain);
		
		if (rain > 0.0) {
			for (size_t i = 1; i <= nSizeY; i++){
				for (size_t j = 1; j <= nSizeX; j++){
					mxRain(i,j)=rain;	
				}
			}
			mxFluid = mxFluid + mxRain;
			accumulated_rain += rain;
		}
		
		MultiflowDMatrix flux_distribution;
		compute_flux_distribution(mxMLDD, mxFluid, flux_distribution);

		MultiflowDMatrix velocity_mldd;
		double time_interval = compute_velocity_mldd(flux_distribution, mxMultiflowAngles, velocity_coefficent,  velocity_mldd);

		MultiflowDMatrix mxOutflowFlux;
		compute_outflow_flux_mldd( velocity_mldd, flux_distribution, time_interval,mxOutflowFlux);

		DblRasterMx mxInFlow;
		DblRasterMx mxOutFlow;
		compute_material_movement(mxOutflowFlux,mxInFlow, mxOutFlow);

		mxFluid = mxFluid - mxOutFlow + mxInFlow;

		double fluid_outside_of_terrain = 0.0;

		for (size_t i = 0; i < nSizeX+2; ++i) {
			fluid_outside_of_terrain +=  mxFluid(0, i);
			fluid_outside_of_terrain += mxFluid(nSizeY+1, i);
		}

		for (size_t i = 1; i <= nSizeY; ++i) {
			fluid_outside_of_terrain +=  mxFluid(i, 0);
			fluid_outside_of_terrain += mxFluid(i, nSizeX + 1);
		}
		fluid_outside_of_terrain*= pixelSize*pixelSize;
		std::cout << "Fluid outside:" << fluid_outside_of_terrain << "/" << rain_full_volume << std::endl;

		/*
		if ( fluid_outside_of_terrain >=rain_full_volume - DoubleUtil::sDeltaD3)
			break;
			*/
		
	}

	return true;
}
}