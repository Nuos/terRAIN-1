#ifndef EXOPERANDSNOTCOMPATIBILE_H
#define EXOPERANDSNOTCOMPATIBILE_H

#include "exception_base.h"

namespace TR
{

class exOperandsNotCompatibile : public  exception_base
{
public:
	exOperandsNotCompatibile():exception_base("Operands are not compatibile!")
	{}
};

}

#endif