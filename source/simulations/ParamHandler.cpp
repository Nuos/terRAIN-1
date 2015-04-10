#include "ParamHandler.h"
#include "ParameterException.h"
#include "ParamNames.h"

namespace SIMULATION
{

string ParamHandler::paramAsString(string paramName) throw (...) {
	
	map<std::string, std::string>::iterator it = _params.find(paramName);
	if (it==_params.end()) {
		string msg("Missing parameter: ");
		msg+=paramName;
		throw ParameterException(msg.c_str());
	}
		
	return it->second;
}

void ParamHandler::addParamString(string name, string value) {
	_params[name] = value;	
}

int ParamHandler::paramAsInteger(string paramName) {
	string strValue = paramAsString(paramName);
	int ret = atoi(strValue.c_str());
	return ret;
}

bool ParamHandler::paramAsBoolean(string paramName) {
	string strValue = paramAsString(paramName);
	
	if (strValue=="1" || strValue=="true" || strValue=="yes" || strValue=="TRUE" || strValue=="YES")
		return true;

	return false;
}

double ParamHandler::paramAsDouble(string paramName) {
	string strValue = paramAsString(paramName);
	double ret = atof(strValue.c_str());
	return ret;
}
	
eOutflowType ParamHandler::getOutflowType() {

	string strValue = paramAsString(OUTFLOW_TYPE);
	
	if (strValue=="topleft") {
		return ofTopLeft;
	} else if (strValue=="topleft") {
		return ofTopLeft;
	} else if (strValue=="topright") {
		return ofTopRight;
	} else if (strValue=="bottomleft") {
		return ofBottomLeft;
	} else if (strValue=="bottomright") {
		return ofBottomRight;
	} else if (strValue=="top") {
		return ofTopSide;
	} else if (strValue=="right") {
		return ofRightSide;
	} else if (strValue=="bottom") {
		return ofBottomSide;
	} else if (strValue=="left") {
		return ofLeftSide;
	} else if (strValue=="all") {
		return ofAllSides;
	} else if (strValue=="topandbottom") {
		return ofTopAndBottomSide;
	} else if (strValue=="leftandright") {
		return ofLeftAndRightSide;
	} else if (strValue=="topandmiddleofbottom") {
		return ofTopAndMiddleOfBottom;
	}
	
	string msg("Invalid outflow type: ");
	msg+=strValue;
	throw ParameterException(msg.c_str());
}


}