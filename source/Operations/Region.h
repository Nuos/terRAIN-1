#ifndef REGION_H
#define REGION_H


namespace TR
{

class Region
{
private:
	unsigned int _nRow1;
	unsigned int _nCol1;
	unsigned int _nRow2;
	unsigned int _nCol2;
public:
	Region(unsigned int nRow1, unsigned int nCol1, unsigned int nRow2, unsigned int nCol2);

	unsigned int getRow1() const 
	{
		return _nRow1;	
	}

	unsigned int getCol1() const 
	{
		return _nCol1;
	}

	unsigned int getRow2() const 
	{
		return _nRow2;
	}

	unsigned int getCol2() const 
	{
		return _nCol2;
	}
};

}

#endif