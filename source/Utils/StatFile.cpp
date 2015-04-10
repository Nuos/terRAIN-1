#include "StatFile.h"
#include "ModelParams.h"
#include "OperationInterface.h"

StatFile::StatFile(const char * lpszName, size_t fieldSize):_fieldSize(fieldSize)
{
	TR::ModelParams params = TR::ModelParams::getModelParamsObj();
	
	std::string strFile(params.getOutputDir());
	strFile+="\\";
	strFile+=lpszName;
	
	_ofs = new std::ofstream(strFile.c_str());

}

StatFile::~StatFile()
{
	delete _ofs;
}

void StatFile::print(const char * lpszStr)
{
	std::string str(lpszStr);
	fillSpaces(str,_fieldSize,' ');
	(*_ofs)<<str<<" ";
}

void StatFile::print(double dbl)
{	
	char buffer[256];
	sprintf(buffer,"%f",dbl);
	print(buffer);
}


void StatFile::print(DblRasterMx & mx)
{
	print((double) mapavarage(mx));
}

void StatFile::endl()
{
	(*_ofs)<<std::endl;
}

void StatFile::fillSpaces(std::string & str, size_t fieldSize, char c)
{
	size_t size = str.size();

	if (fieldSize>size){
		std::string strTmp(fieldSize - size,c);
		str=strTmp+str;
		return;
	}
	
	if (size>fieldSize)
		str = str.substr(0,fieldSize);
	
}
