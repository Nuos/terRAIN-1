#ifndef EXPARALELLIZE_H
#define EXPARALELLIZE_H

#include "exception_base.h"

namespace TR
{
	
	class exParalellize : public exception_base
	{
	public:
		exParalellize() : exception_base("Critical error in paralellization"){}
	};
}

#endif 