#ifndef LONGESTFLOWPATHLENGTHDEFS_H
#define LONGESTFLOWPATHLENGTHDEFS_H

#include "longestFlowPathLengthFunctors.h"
#include "longestFlowPathLengthFunc.h"

namespace TR
{

	typedef XLongestFlowPathLengthFuncT<DblRasterMx,FUNC::funcLongestFlowPathLengthLDD> funcLongestFlowPathLengthLDD;
	typedef XLongestFlowPathLengthFuncT<MultiflowDMatrix,FUNC::funcLongestFlowPathLengthMLDD> funcLongestFlowPathLengthMLDD;

}
#endif