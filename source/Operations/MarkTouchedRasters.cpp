#include "MarkTouchedRasters.h"
#include "MarkIfNotMarked.h"

namespace TR
{

MarkTouchedRasters::MarkTouchedRasters(DblRasterMx & markMx, double mark):
_markMx(markMx), _mark(mark), _cnt(0), condition(new MarkIfNotMarked())
{}

bool MarkTouchedRasters::operator()(const MultiflowDMatrix::iterator & it)
{
	size_t row = it.getRow();
	size_t col = it.getCol();

	if (condition->check(row,col,_markMx)){
		_markMx(row, col) = _mark;
		++_cnt;
		return true;
	}

	return false;
}

size_t MarkTouchedRasters::getCnt() const
{
	return _cnt;
}

void MarkTouchedRasters::setMark(double mark)
{
	_mark = mark;
}

void MarkTouchedRasters::inc(double d)
{
	_mark+=d;
}

MarkTouchedRasters::~MarkTouchedRasters()
{
	delete condition;
}

void MarkTouchedRasters::setCondition(MarkCondition * cond)
{
	delete condition;
	condition = cond;
}

}