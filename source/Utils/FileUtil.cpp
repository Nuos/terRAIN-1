#include "FileUtil.h"
#include "ModelParams.h"

void FileUtil::CreateFilePath(std::string & strBaseName, size_t nIter, eFileType fileType, std::string & strFilePath)
{
	TR::ModelParams params = TR::ModelParams::getModelParamsObj();

	char lpszTmp[256];

	sprintf(lpszTmp,"%s\\%s%06u.%s",params.getOutputDir().c_str(), strBaseName.c_str(),nIter,
		fileType==filetypeCol ? "col" : "asc");

	strFilePath = lpszTmp;
} 