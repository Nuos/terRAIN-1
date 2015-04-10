#ifndef PARAMETER_EXCEPTION_H
#define PARAMETER_EXCEPTION_H

#include "exception_base.h"

class ParameterException : public exception_base
{
public:
	ParameterException(const char * lpszWhat) : exception_base(lpszWhat){}
};

#endif