#ifndef RASTERPOSITION_H
#define RASTERPOSITION_H
#include <ostream>
namespace TR
{

class RasterPosition
{
private:
	size_t _row;
	size_t _col;
public:
	RasterPosition():_row(0), _col(0){}
	RasterPosition(size_t row, size_t col) :_row(row), _col(col){}

	inline void set(size_t row, size_t col) {
		_row = row;
		_col = col;
	}

	inline size_t getCol() const
	{
		return _col;
	}

	size_t getRow() const
	{
		return _row;
	}

	friend std::ostream & operator<<(std::ostream & os, RasterPosition & rasterPos) 
	{
		os<<"[ ";
		os<< rasterPos.getRow();
		os<<",";
		os<< rasterPos.getCol();
		os<<" ]";
		
		return os;
	}

};

}

#endif