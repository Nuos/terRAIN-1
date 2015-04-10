#ifndef ACCFLUXDEFS_H
#define ACCFLUXDEFS_H

#include "accfluxFuncT.h"
#include "accfluxfunctions.h"

using namespace FUNC;

namespace TR
{
	typedef accfluxFuncT<DblRasterMx,funcAccFluxOnLDD> accfluxFuncOnLDD;
	//typedef accfluxFuncT<MultiflowDMatrix,funcAccFluxOnMLDD> accfluxFuncOnMLDD;
	typedef accfluxFuncT<DblRasterMx,funcLoopCheck> loopCheckFuncOnLDD;
}

#endif