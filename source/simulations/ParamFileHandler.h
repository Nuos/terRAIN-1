#ifndef PARAMFILE_HANDLER_H
#define PARAMFILE_HANDLER_H

#include "ParamHandler.h"

namespace SIMULATION
{

class ParamFileHandler : public ParamHandler
{
public:
	ParamFileHandler(){}
	void readParamFile(const char * file);
};

}


#endif