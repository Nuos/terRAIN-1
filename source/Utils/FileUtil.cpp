#include "FileUtil.h"
#include "ModelParams.h"

void FileUtil::CreateFilePath(std::string & strBaseName, size_t nIter, eFileType fileType, std::string & strFilePath)
{
	TR::ModelParams params = TR::ModelParams::getModelParamsObj();

	char lpszTmp[256];

	std::string extension;
	switch (fileType)
	{
	case filetypeCol:
		extension = "col";
		break;
	case filetypeAscii:
		extension = "asc";
		break;
	case filetypeArcGis:
		extension = "asc";
		break;
	}
	sprintf(lpszTmp,"%s\\%s%06u.%s",params.getOutputDir().c_str(), strBaseName.c_str(),nIter,
		extension.c_str());

	strFilePath = lpszTmp;
} 