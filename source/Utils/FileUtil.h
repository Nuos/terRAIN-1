#ifndef FILEUTIL_H
#define FILEUTIL_H

#include "defs.h"

#include <string>

class FileUtil
{
public:
	static void CreateFilePath(std::string & strBaseName, size_t nIter, eFileType fileType, std::string & strFilePath);	
};

#endif