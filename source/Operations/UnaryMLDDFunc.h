#ifndef UNARYMLDDFUNC_H
#define UNARYMLDDFUNC_H

#include "paralellisable.h"
#include "MultiflowDMatrix.h"
#include "rastermatrix.h"

namespace TR
{

class UnaryMLDDFunc : public paralellisable
{

protected:
	MultiflowDMatrix::iterator _OpBeginIter;
	MultiflowDMatrix::iterator _OpEndIter;
	DblRasterMx::iterator _RetIterator;
public:
	UnaryMLDDFunc( MultiflowDMatrix::iterator & OpBeginIter, MultiflowDMatrix::iterator & OpEndIter, 
		DblRasterMx::iterator & RetIterator);

	MultiflowDMatrix::iterator & getOpBegin();
	MultiflowDMatrix::iterator & getOpEnd();
	DblRasterMx::iterator getRetIterator();
	void setOpBegin(const MultiflowDMatrix::iterator & OpBegin);
	void setOpEnd(const MultiflowDMatrix::iterator & OpEnd);
	void setRetIterator(DblRasterMx::iterator & retIterator);

};

}


#endif
