# include "UnaryMLDDFunc.h"

namespace TR
{

UnaryMLDDFunc::UnaryMLDDFunc( MultiflowDMatrix::iterator & OpBeginIter, MultiflowDMatrix::iterator & OpEndIter, 
		DblRasterMx::iterator & RetIterator):_OpBeginIter(OpBeginIter), _OpEndIter(OpEndIter),
		_RetIterator(RetIterator)
{}
	
MultiflowDMatrix::iterator & UnaryMLDDFunc::getOpBegin() {
	return _OpBeginIter;
}

MultiflowDMatrix::iterator & UnaryMLDDFunc::getOpEnd() {
	return _OpEndIter;
}

DblRasterMx::iterator UnaryMLDDFunc::getRetIterator() {
	return _RetIterator;
}

void UnaryMLDDFunc::setOpBegin(const MultiflowDMatrix::iterator & OpBegin) {
	_OpBeginIter = OpBegin;
}

void UnaryMLDDFunc::setOpEnd(const MultiflowDMatrix::iterator & OpEnd) {
	_OpEndIter = OpEnd;
}

void UnaryMLDDFunc::setRetIterator(DblRasterMx::iterator & retIterator) {
	_RetIterator = retIterator;
}

}