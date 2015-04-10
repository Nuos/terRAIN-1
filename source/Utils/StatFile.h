#ifndef STATFILE_H
#define STATFILE_H

#include <fstream>
#include <string>
#include "rastermatrix.h"


using namespace TR;

class StatFile
{
private:
	std::ofstream * _ofs;
	size_t _fieldSize;
public:
	StatFile(const char * lpszName, size_t fieldSize = 20);
	~StatFile();

	void print(const char * lpszStr);
	void print(double dbl);
	void print(DblRasterMx & mx);
	void endl();

	static void fillSpaces(std::string & str, size_t fieldSize, char c);
};



#endif