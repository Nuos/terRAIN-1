#ifndef ITEMDESCRIPTOR_H
#define ITEMDESCRIPTOR_H

#include "MultiflowDMatrix.h"

namespace TR
{

class ItemDescriptor
{
public:
	unsigned char				_nChainCode;
	MultiflowDMatrix::iterator	_iLDD;
	double						_dblValue;

	ItemDescriptor( const MultiflowDMatrix::iterator & iLDD, double dblValue ): _nChainCode(1),_iLDD(iLDD), _dblValue(dblValue)
	{}
	
	unsigned char incChainCode()
	{
		_nChainCode++;

		if (_nChainCode==5)
			_nChainCode++;

		return _nChainCode;	
	}
};

}
#endif