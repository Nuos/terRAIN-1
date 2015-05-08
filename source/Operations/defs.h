#ifndef DEFS_H
#define DEFS_H

#define _USE_MATH_DEFINES
#include <cmath>

namespace TR
{

enum eOrigo
{
	origoTopLeft = 0,
	origoTopRight,
	origoBottomLeft,
	origoBottomRight
};

enum eOutflowType
{
	ofTopLeft  = 0,
	ofTopRight,
	ofBottomLeft,
	ofBottomRight,
	ofTopSide,
	ofRightSide,
	ofBottomSide,
	ofLeftSide,
	ofAllSides,
	ofTopAndBottomSide,
	ofLeftAndRightSide,
	ofTopAndMiddleOfBottom
};


#define RadToDeg  180 * M_1_PI
#define DegToRad  M_PI/180.0

const double SQRT2 = 1.4142135623730950488016887242097;
const double CARDDIAGDELTA = 0.29289321881345247559915563789515; //(1 - 1/SQRT2)
const double SQRT2REC = 1/SQRT2;
enum eCoordType
{
	ctX = 0,
	ctY
};

}

enum eFileType
{
	filetypeCol = 0,
	filetypeAscii,
	filetypeArcGis,

};
#endif