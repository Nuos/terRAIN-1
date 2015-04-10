#ifndef EXDIFFERENTMATRICES_H
#define EXDIFFERENTMATRICES_H

#include "exception_base.h"
namespace TR
{
class exDifferentMatrices: public exception_base
{
public:
	exDifferentMatrices() : exception_base("Iterators are associeted to different matrices")
	{}
};

}


#endif