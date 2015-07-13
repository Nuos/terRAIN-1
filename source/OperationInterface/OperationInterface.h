#ifndef OPERATIONINTERFACE_H
#define OPERATIONINTERFACE_H

#include "SimpleMxOperations.h"
#include "defs.h"
#include "MultiflowDMatrix.h"
#include "RasterPositionMatrix.h"
#include "FileUtil.h"
#include <fstream>


namespace TR
{

void setOutflowType(eOutflowType outflowType);

void mapattr(size_t nRows, size_t nCols, double dblPixelSize, double dblInitValue, DblRasterMx & mxInit);

double mapavarage(DblRasterMx & mxOp);

void add(  DblRasterMx & mxOp1,  DblRasterMx & mxOp2, DblRasterMx & mxRet );
void sup(  DblRasterMx & mxOp1,  DblRasterMx & mxOp2, DblRasterMx & mxRet );
void mul(  DblRasterMx & mxOp1,  DblRasterMx & mxOp2, DblRasterMx & mxRet );
void fraction(  DblRasterMx & mxOp1,  DblRasterMx & mxOp2, DblRasterMx & mxRet );
void power(  DblRasterMx & mxOp1,  DblRasterMx & mxOp2, DblRasterMx & mxRet );

void cover(  DblRasterMx & mxOp1,  DblRasterMx & mxOp2, DblRasterMx & mxRet );
void cover(  DblRasterMx & mxOp1, double dblProxy, DblRasterMx & mxRet );

void abs(  DblRasterMx & mxOp, DblRasterMx & mxRet );

void sin(  DblRasterMx & mxOp, bool bRad, DblRasterMx & mxRet );
void cos(  DblRasterMx & mxOp, bool bRad, DblRasterMx & mxRet  );
void tg(  DblRasterMx & mxOp, bool bRad,  DblRasterMx & mxRet );
void ctg(  DblRasterMx & mxOp, bool bRad, DblRasterMx & mxRet);
void asin(  DblRasterMx & mxOp,bool bRad, DblRasterMx & mxRet);
void acos(  DblRasterMx & mxOp,bool bRad, DblRasterMx & mxRet );
void atg(  DblRasterMx & mxOp,  bool bRad, DblRasterMx & mxRet);
void actg(  DblRasterMx & mxOp,bool bRad, DblRasterMx & mxRet);

void ln(  DblRasterMx & mxOp, DblRasterMx & mxRet);
void log10(  DblRasterMx & mxOp, DblRasterMx & mxRet);
void exp(  DblRasterMx & mxOp, double dblBase, DblRasterMx & mxRet);

void xcoordinate( DblRasterMx & mxOp, DblRasterMx & mxRet);
void ycoordinate( DblRasterMx & mxOp, DblRasterMx & mxRet);

void uniform( DblRasterMx & mxRet );

void slope(  DblRasterMx & mxOp, DblRasterMx & mxRet );
void lddcreate(  DblRasterMx & mxOp, DblRasterMx & mxRet, bool bFillPits );
void findpits(  DblRasterMx & mxOp, DblRasterMx & mxRet );
//void fillpits(  DblRasterMx & mxLDD,DblRasterMx & mxRasters, DblRasterMx & mxRet );
void multiflowLDD(double a, DblRasterMx & mxOp, MultiflowDMatrix & mxRet, bool bFillPits);
void multiflowLDD(DblRasterMx a, DblRasterMx & mxOp, MultiflowDMatrix & mxRet, bool bFillPits);
void semiMultiflowLDD(DblRasterMx & mxOp, MultiflowDMatrix & mxRet, bool bFillPits);
void accflux( DblRasterMx & mxLDD, DblRasterMx & mxDepth, DblRasterMx & mxRet);
//ez csak teszt volt void check_loop( DblRasterMx & mxLDD, DblRasterMx & mxRet);
void accflux( MultiflowDMatrix & mxLDD, DblRasterMx & mxDepth, DblRasterMx & mxRet, double dblFluxLimitRate);

void upstreamtotal( DblRasterMx & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet, bool bDiagonal);
void upstreamtotal( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,bool bDiagonal,bool inFlow);
void upstreammax( DblRasterMx & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,RasterPositionMatrix & mxPos);
void upstreammax( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet, RasterPositionMatrix & mxPos, bool inFlow);
void upstreammin( DblRasterMx & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet, RasterPositionMatrix & mxPos);
void upstreammin( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,RasterPositionMatrix & mxPos,bool inFlow);

void downstreamtotal( DblRasterMx & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet);
void downstreamtotal( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet);
void downstreammin( DblRasterMx & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,RasterPositionMatrix & mxPos,bool inFlow);
void downstreammin( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,RasterPositionMatrix & mxPos,bool inFlow);
void downstreammax( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet,RasterPositionMatrix & mxPos,bool inFlow);

void longestflowpathlength(DblRasterMx & mxLDD, DblRasterMx & mxRet);
void longestflowpathlength(MultiflowDMatrix & mxLDD, DblRasterMx & mxRet);
void min( DblRasterMx & mxOp1, DblRasterMx & mxOp2, DblRasterMx & mxRet );
void max( DblRasterMx & mxOp1, DblRasterMx & mxOp2, DblRasterMx & mxRet );

void setOutputDirectory(const char * lpszOutputDir);
void diagonal(MultiflowDMatrix & mxLDD,DblRasterMx & mxRet);

void display(DblRasterMx & mx);
void displayInterpolate(DblRasterMx & mx);

void signStreamPixels(DblRasterMx & mxOp, DblRasterMx & mxRet,double mul);
void signStreamPixels(MultiflowDMatrix & mxLDD, DblRasterMx & mxRet,double mul);
void signStreamPixels(DblRasterMx & mxOp,MultiflowDMatrix & mxLDD, DblRasterMx & mxRet,double mul);

void windowAverage(DblRasterMx & mxOp, DblRasterMx & mxRet,double windowLength);
void windowMin(DblRasterMx & mxOp, DblRasterMx & mxRet,double windowLength);
void windowMax(DblRasterMx & mxOp, DblRasterMx & mxRet,double windowLength);
void windowTotal(DblRasterMx & mxOp, DblRasterMx & mxRet,double windowLength);
void windowVariance(DblRasterMx & mxOp, DblRasterMx & mxRet,double windowLength);

void path( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet);
void catchment( MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet);

void getMatrixPosItems(DblRasterMx & mxOp,RasterPositionMatrix & mxPos,DblRasterMx & mxTarget);  
void diagonal(RasterPositionMatrix & mxPos,DblRasterMx & mxRet);
void spreadLDD( MultiflowDMatrix & mxLDD, DblRasterMx & mxPoints, DblRasterMx & mxRet,double dblFluxLimitRate );
void spreadLDDMax( MultiflowDMatrix & mxLDD, DblRasterMx & mxPoints, DblRasterMx & mxRet,double dblFluxLimitRate );
void findOutflowPITs(DblRasterMx & mx,DblRasterMx & ret);
void compute_runoff_distribution( MultiflowDMatrix & mxVelocity, DblRasterMx & flowDepth, MultiflowDMatrix & mxRet);


struct erosion_rate_params
{
	double runoff_exponent;
	double slope_exponent; 
	DblRasterMx fluvial_const;
	DblRasterMx diffusive_const; 
	double min_elevation_diff;
	double critical_slope;
	double diffusion_exponent;
	double infinite_erosion_rate;
	bool simple_diffusion;

	erosion_rate_params();
};

struct erosion_rate_results
{
	MultiflowDMatrix mxDiffusiveErosionRate;
	MultiflowDMatrix mxFluvialErosionRate;
};


double compute_erosion_rate(const erosion_rate_params & params, DblRasterMx & terrain, MultiflowDMatrix & runoff_distr, MultiflowDMatrix & mldd_slope, erosion_rate_results & res);
void compute_sediment_flux(MultiflowDMatrix & mxSedimentVelocityMLDD, double dt, MultiflowDMatrix & mxRet);


template<class T> 
void saveToCol(rastermatrix<T> & mx, size_t nIter, const char * lpszBaseName)
{

	std::string strBase(lpszBaseName);
	std::string strColFile;
	
	FileUtil::CreateFilePath(strBase,nIter,filetypeCol,strColFile);

	std::ofstream ofs(strColFile.c_str());

	mx.writeToColFormat(ofs);
}

template<class T> 
void saveToAsc(rastermatrix<T> & mx, size_t nIter, const char * lpszBaseName, bool removeOutflowSides = false)
{
	std::string strBase(lpszBaseName);
	std::string strAscFile;
	
	FileUtil::CreateFilePath(strBase,nIter,filetypeAscii,strAscFile);

	std::ofstream ofs(strAscFile.c_str());

	mx.writeToAsc(ofs,removeOutflowSides);
}

template<class T>  
void loadFromCol(const char * lpszFileName, rastermatrix<T> & mx)
{	
	
	std::ifstream isf(lpszFileName);
	
	mx.readFromColFormat(isf);
		
}

template<class T> 
void loadFromAsc(const char * lpszFileName, rastermatrix<T> & mx, double dblPixelSize)
{
	
	std::ifstream isf(lpszFileName);
	mx.readFromAsc(isf,dblPixelSize);

}

void sum_of_downstreamdiffs(MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet, double limitRate);
void sum_of_upstreamdiffs(MultiflowDMatrix & mxLDD, DblRasterMx & mx, DblRasterMx & mxRet, double limitRate);


DblRasterMx operator+(DblRasterMx & op1, DblRasterMx & op2);
DblRasterMx operator-(DblRasterMx & op1, DblRasterMx & op2);
DblRasterMx operator/(DblRasterMx & op1, DblRasterMx & op2);
DblRasterMx operator*(DblRasterMx & op1, DblRasterMx & op2);
DblRasterMx operator^(DblRasterMx & op1, DblRasterMx & op2);

bool compute_flux_distribution(MultiflowDMatrix & mxLDD, DblRasterMx & mxFlux, MultiflowDMatrix & mxRet);
void multiflowAngles(DblRasterMx & mxOp, MultiflowDMatrix & mxRet, bool bFillPits);
double compute_velocity_mldd(MultiflowDMatrix & fluxDistribution, MultiflowDMatrix & flowAngles, double c,  MultiflowDMatrix & mxVelocity);
void compute_outflow_flux_mldd( MultiflowDMatrix & mxVelocity, MultiflowDMatrix & fluxDistribution, double dt, MultiflowDMatrix & mxRet);
void compute_material_movement(MultiflowDMatrix & mxOutFlowFlux, DblRasterMx & mxRetInFlow, DblRasterMx & mxRetOutFlow);
void shift_right(DblRasterMx & mx);
void shift_left(DblRasterMx & mx);

enum SpecialPoint
{
	notSpecPoint = 0,
	ridge = 1,
	peak = 1 << 1,
	col = 1 << 2, //nyereg
	channel = 1 << 3, // bevágódás, metszõdés
	slope_point = 1 << 4,
};

void find_special_points(DblRasterMx & terrain, unsigned int spec_points, IntRasterMx & ret);

template<class T> 
void saveToArcgis(rastermatrix<T> & mx, size_t nIter, const char * lpszBaseName)
{
	std::string strBase(lpszBaseName);
	std::string strArcgisFile;
	
	FileUtil::CreateFilePath(strBase,nIter,filetypeAscii,strArcgisFile);

	std::ofstream ofs(strArcgisFile.c_str());
	size_t ncols = mx.getColNr();
	size_t nrows = mx.getColNr();
	double pixelSize = mx.getPixelSize();
	ofs << "ncols         " << ncols << std::endl;
	ofs << "nrows         " << nrows << std::endl;
	ofs << "xllcorner     " << 0.0 << std::endl;
	ofs << "yllcorner     " << 0.0 << std::endl;
	ofs << "cellsize          " << pixelSize << std::endl;
	ofs << "NODATA_value  -9999" << std::endl;

	rastermatrix<T>::iterator iMx = mx.begin(), endMx = mx.end();
	rastermatrix<T>::iterator begin = mx.begin();
	for (; iMx != endMx; ++iMx) {
		double val = *iMx;
		if (iMx != begin)
			ofs << " ";
		ofs << val;
	}
}

bool loadFromArcgis(const char * lpszFileName, DblRasterMx & mx);

void distanceTransform(const DblRasterMx & mx, DblRasterMx & distances, RasterPositionMatrix & positions);
void create_sample_terrain(const DblRasterMx & fixed_heights, bool valleys, double mul, DblRasterMx & sample_terrain);

}

#endif