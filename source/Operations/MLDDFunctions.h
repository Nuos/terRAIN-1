#ifndef MLDDFUNCTIONS_H
#define MLDDFUNCTIONS_H

#include "MultiflowDMatrix.h"
#include "MarkFunction.h" 
#include "MLDDWalkFunction.h"
#include <list>
namespace TR
{

class MLDDFunctions
{
public:
	static void walkBackOnFlow(MultiflowDMatrix::iterator iter, MLDDWalkFunction & func, std::list<MultiflowDMatrix::iterator> & maxPoints);
	static void walkOnFlow(MultiflowDMatrix::iterator iter, MLDDWalkFunction & func, std::list<MultiflowDMatrix::iterator> & minPoints);

};

}

#endif