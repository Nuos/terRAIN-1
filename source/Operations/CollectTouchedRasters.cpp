#include "CollectTouchedRasters.h"
#include "MarkIfNotMarked.h"

namespace TR {

CollectTouchedRasters::CollectTouchedRasters(DblRasterMx & markMx, std::vector<MultiflowDMatrix::iterator> & collection):
_markMx(markMx), _collection(collection), _condition(new MarkIfNotMarked())
{}

bool CollectTouchedRasters::operator()(const MultiflowDMatrix::iterator & it)
{
	size_t row = it.getRow();
	size_t col = it.getCol();

	if (_condition->check(row,col,_markMx)){
		_collection.push_back(it);
		return true;
	}

	return false;
}

CollectTouchedRasters::~CollectTouchedRasters()
{
	delete _condition;
}

void CollectTouchedRasters::setCondition(MarkCondition * cond)
{
	delete _condition;
	_condition = cond;
}

}