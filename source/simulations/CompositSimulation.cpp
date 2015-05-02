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


enum GroundProductionType
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
	GroundProductionType groundProductionType = gpNone;
	RunoffProductionType runoffProductionType = rfRainfallRunoff;


	// common variables
	size_t nSizeX = 20;
	size_t nSizeY = 20;
	double pixelSize = 10;
	double rainIntensity = pixelSize/10000; // [length_unit/time_unit]
	double amount_of_rain = pixelSize/100; // [length_unit]
	double accumulated_rain = 0.0; //[length_unit]
	double iteration_time = 1.0; // [time_unit]
	DblRasterMx mxRain;
	mapattr(nSizeY+2,nSizeX+2,pixelSize,0.0, mxRain);
	DblRasterMx mxFlowDepth;
	mapattr(nSizeY+2,nSizeX+2,pixelSize,0.0, mxFlowDepth);

	// base terrain layer (rock based)
	DblRasterMx terrainBase;
	// graound layer above the rock based terrain layer 
	DblRasterMx ground;

	//initial terrain and ground 
	
	switch (groundProductionType)
	{
		case gpNone:
		{
			mapattr(nSizeY+2,nSizeX+2,pixelSize,0.0, terrainBase);

			// initial ground generation
			double slope = 1.0;
			double elevation = 100;

			mapattr(nSizeY+2,nSizeX+2,pixelSize,0.0, ground);
			for (size_t i = 1; i < nSizeX; i++){
				for (size_t j = 1; j <= nSizeY; j++){
					ground(i,j)=elevation + (i+j)*slope;	
				}
			}
			break;
		}
		default:
			std::cout << "Invalid ground type production type: " << groundProductionType;
			return false;
	}


	bool stopCondition = true;

	// params of rfRainfallRunoff
	double velocity_coefficent = pixelSize*pixelSize;
	double rain_full_volume = amount_of_rain * pixelSize * pixelSize * nSizeX * nSizeY;

	while (stopCondition) 
	{
		switch (runoffProductionType)
		{
			case rfRainfallRunoff:
			{
				double rain = iteration_time * rainIntensity;
				rain = min(rain, amount_of_rain - accumulated_rain);
		
				if (rain > 0.0) {
					for (size_t i = 1; i <= nSizeY; i++){
						for (size_t j = 1; j <= nSizeX; j++){
							mxRain(i,j)=rain;	
						}
					}
					mxFlowDepth = mxFlowDepth + mxRain;
					accumulated_rain += rain;
				}
			
			}
		}

	}



	return true;
}

}