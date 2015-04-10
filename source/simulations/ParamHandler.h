#ifndef PARAMHANDLER_H
#define PARAMHANDLER_H

#include <string>
#include <map>
#include "defs.h"

using namespace std;
using namespace TR;

namespace SIMULATION
{
//! \class Base class of parameter handlers
class ParamHandler
{
protected:
	//! parameter name -> parameter value map
	map<std::string, std::string> _params;
public:
	ParamHandler(){}
	
	//! \param [IN] paramName is the name of the parameter
	//! \return string value of the parameter
	string paramAsString(string paramName);

	//! \param [IN] paramName is the name of the parameter
	//! \return integer parameter value
	int paramAsInteger(string paramName);

	//! \param [IN] paramName is the name of the parameter
	//! \return boolean parameter value
	bool paramAsBoolean(string paramName);

	//! \param [IN] paramName is the name of the parameter
	//! \return double parameter value
	double paramAsDouble(string paramName);

	//! \return outflow type if set in parameters, otherwise throw ParameterException
	eOutflowType getOutflowType();

	//! Add a new parameter
	//! \param [IN] name is the name of the parameter
	//! \value [IN] if the value of the parameter
	void addParamString(string name, string value);
};

}

#endif