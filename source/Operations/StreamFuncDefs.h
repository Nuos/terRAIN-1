#ifndef STREAMFUNCDEFS_H
#define STREAMFUNCDEFS_H

#include "XStreamFuncT.h"
#include "MultiflowDMatrix.h"
#include "StreamFunctors.h"

namespace TR
{
	typedef XStreamFuncT<DblRasterMx,FUNC::funcUpStreamTotalLDD> mfLDDUpStreamTotal;
	typedef XStreamFuncT<MultiflowDMatrix,FUNC::funcUpStreamTotalMLDD> mfMLDDUpStreamTotal;
	typedef XStreamFuncT<DblRasterMx,FUNC::funcUpStreamMaxLDD> mfLDDUpStreamMax;
	typedef XStreamFuncT<MultiflowDMatrix,FUNC::funcUpStreamMaxMLDD> mfMLDDUpStreamMax;
	typedef XStreamFuncT<DblRasterMx,FUNC::funcUpStreamMinLDD> mfLDDUpStreamMin;
	typedef XStreamFuncT<MultiflowDMatrix,FUNC::funcUpStreamMinMLDD> mfMLDDUpStreamMin;


	typedef XStreamFuncT<DblRasterMx,FUNC::funcDownStreamTotalLDD> mfLDDDownStreamTotal;
	typedef XStreamFuncT<MultiflowDMatrix,FUNC::funcDownStreamTotalMLDD> mfMLDDDownStreamTotal;
	typedef XStreamFuncT<MultiflowDMatrix,FUNC::funcDownStreamMinMLDD> mfMLDDDownStreamMin;
	typedef XStreamFuncT<MultiflowDMatrix,FUNC::funcDownStreamMaxMLDD> mfMLDDDownStreamMax;
}

#endif