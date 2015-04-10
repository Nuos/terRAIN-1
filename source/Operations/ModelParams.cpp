#include "ModelParams.h"

namespace TR
{

ModelParams & ModelParams::getModelParamsObj()
{
	static ModelParams objModelParams;

	return objModelParams;
}

eOutflowType ModelParams::getOutflowType() const
{
	return _OutflowType;
}

void ModelParams::setOutflowType( eOutflowType ofType )
{
	_OutflowType = ofType;
}

ModelParams::ModelParams(): _OutflowType(ofAllSides),_strOutputDir(".")
{

}

std::string ModelParams::getOutputDir() const
{
	return _strOutputDir;
}

void ModelParams::setOutputDir( std::string & strOutputDir )
{
	_strOutputDir = strOutputDir;
}

}