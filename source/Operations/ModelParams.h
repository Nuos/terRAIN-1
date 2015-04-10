#ifndef MODELPARAMS_H
#define MODELPARAMS_H

#include "defs.h"
#include <string>

namespace TR
{

class ModelParams
{
public:
	static ModelParams & getModelParamsObj();
	
	eOutflowType getOutflowType() const;
	void setOutflowType( eOutflowType ofType );

	std::string getOutputDir() const;
	void setOutputDir( std::string & strOutputDir );

private:
	ModelParams();

	eOutflowType _OutflowType;
	std::string  _strOutputDir;

};

}

#endif