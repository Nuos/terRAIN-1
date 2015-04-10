#ifndef SIMULATIONEXCEPTION_H
#define SIMULATIONEXCEPTION_H

#include "exception_base.h"

class SimulationException : public exception_base
{
public:
	SimulationException(const char * lpszWhat) : exception_base(lpszWhat){}
};

#endif