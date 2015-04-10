#ifndef MARKIFNOTMARKED_H
#define MARKIFNOTMARKED_H

#include "MarkCondition.h"

namespace TR
{
class MarkIfNotMarked : public MarkCondition
{
public:
	virtual bool check(size_t row, size_t col, DblRasterMx & mx)
	{
		return ( mx(row,col)==0.0 );
	}
};

}


#endif