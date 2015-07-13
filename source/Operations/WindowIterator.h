#ifndef WINDOWITERATOR_H
#define WINDOWITERATOR_H

#include "exDifferentMatrices.h"
#include "ModelParams.h"

namespace TR
{

template <class T>
class WindowIterator
{
	typedef typename T::typeData typeData;

protected:
	T * _pOwner;
	typeData * _pData;
	size_t _nRow;
	size_t _nCol;

public:
	struct dCoords
	{
		int _nDRow;
		int _nDCol;
		
		dCoords()
		{
			_nDRow = 0;
			_nDCol = 0;
		}

	};

	dCoords getChainCodeDelta(unsigned char nChainCode)const 
	{
		
		dCoords delta;
		
		switch (nChainCode) {
			case 1:
				delta._nDRow = -1;
				delta._nDCol = -1;
			break;
			case 2:
				delta._nDRow = -1;
				delta._nDCol = 0;
			break;
			case 3:
				delta._nDRow = -1;
				delta._nDCol =  1;
			break;
			case 4:
				delta._nDRow = 0;
				delta._nDCol = -1;
			break;
			case 6:
				delta._nDRow = 0;
				delta._nDCol = 1;
			break;
			case 7:
				delta._nDRow = 1;
				delta._nDCol = -1;
			break;
			case 8:
				delta._nDRow = 1;
				delta._nDCol = 0;
			break;
			case 9:
				delta._nDRow = 1;
				delta._nDCol = 1;
			break;
		}

		return delta;
	}



	WindowIterator(): _pOwner(NULL), _pData(NULL), _nRow(0), _nCol(0) {}
	WindowIterator(T * pOwner, typeData * pData, size_t nRow, size_t nCol): _pOwner(pOwner), _pData(pData), _nRow(nRow), _nCol(nCol) {}
	
	const typeData & operator*() const
	{
		return *_pData;
	}

	typeData & operator*()
	{
		return *_pData;
	}

	const typeData * operator->() const
	{
		return _pData;
	}
	
	typeData * operator->()
	{
		return _pData;
	}

	const typeData & operator()(int nDeltaRow, int nDeltaCol) const
	{
		return _pOwner->operator()(_nRow+nDeltaRow , _nCol+nDeltaCol);
	}

	typeData & operator()(int nDeltaRow, int nDeltaCol)
	{
		return _pOwner->operator()(_nRow+nDeltaRow , _nCol+nDeltaCol);
	}

	typeData & chain_code( unsigned char nChainCode ) const
	{
		dCoords delta = getChainCodeDelta(nChainCode);

		return _pOwner->operator()(_nRow+delta._nDRow , _nCol+delta._nDCol);
	}

	bool chain_code_safe( unsigned char nChainCode , typeData & ret) const 
	{
		dCoords delta = getChainCodeDelta(nChainCode);

		return getItemSafe(delta._nDRow,delta._nDCol,ret);
	}

	bool neighbourIterator(unsigned char nChainCode,WindowIterator<T> & ret) const
	{
		dCoords delta = getChainCodeDelta(nChainCode);

		int nRow = static_cast<int>(_nRow) + delta._nDRow;
		int nCol = static_cast<int>(_nCol) + delta._nDCol;

		if ( nRow < 0 || nRow >= static_cast<int>(_pOwner->getRowNr()) || 
			 nCol < 0 || nCol >= static_cast<int>(_pOwner->getColNr()) )
			return false;

		ret = _pOwner->getIteratorAt(nRow,nCol);

		return true;
	}
	
	bool isValidItemByChainCode( unsigned char nChainCode ) const
	{
		dCoords delta = getChainCodeDelta(nChainCode);

		int nRow = static_cast<int>(_nRow) + delta._nDRow;
		int nCol = static_cast<int>(_nCol) + delta._nDCol;

		if ( nRow < 0 || nRow >= static_cast<int>(_pOwner->getRowNr()) || 
			 nCol < 0 || nCol >= static_cast<int>(_pOwner->getColNr()) )
			return false;

		return true;
	}
	
	
	WindowIterator<T> & operator++()
	{
		_pData++;
		_nCol++;

		if (_pOwner->getColNr()<=_nCol){
			_nCol=0;
			_nRow++;
		}

		return *this;
	}

	WindowIterator<T> & operator--()
	{
		_pData--;
		_nCol--;

		if (_nCol<0){
			_nCol=_pOwner->getColNr()-1;
			_nRow--;
		}

		return *this;
	}

	WindowIterator<T> operator++(int)
	{
		WindowIterator<T> iSave(*this);

		_pData++;
		_nCol++;

		if (_pOwner->getColNr()<=_nCol){
			_nCol=0;
			_nRow++;
		}

		return iSave;
	}

	WindowIterator<T>  operator--(int)
	{
		WindowIterator<T> iSave(*this);
		
		_pData--;
		_nCol--;

		if (_nCol<0){
			_nCol=_pOwner->getColNr()-1;
			_nRow--;
		}

		return iSave;
	}

	T & getOwner()
	{
		return *_pOwner;
	}

	size_t getCol() const
	{
		return _nCol;
	}

	size_t getRow() const
	{
		return _nRow;
	}
	
	bool getItemSafe(int nDeltaRow, int nDeltaCol, typeData & ret) const
	{
		int nRow = static_cast<int>(_nRow) + nDeltaRow;
		int nCol = static_cast<int>(_nCol) + nDeltaCol;

		if ( nRow < 0 || nRow >= static_cast<int>(_pOwner->getRowNr()) || 
			 nCol < 0 || nCol >= static_cast<int>(_pOwner->getColNr()) )
			return false;
		
		ret = _pOwner->operator()(nRow , nCol);

		return true;
	}
	
	bool setItemSafe(int nDeltaRow, int nDeltaCol, const typeData & ret) const
	{
		int nRow = _nRow + nDeltaRow;
		int nCol = _nCol + nDeltaCol;

		if ( nRow < 0 || nRow>=_pOwner->getRowNr() || nCol < 0 || nCol>=_pOwner->getColNr() )
			return false;
		
		_pOwner->operator()(nRow , nCol)=ret;

		return true;
	}
	
	bool operator==(const WindowIterator<T> & rhs) const
	{
		if ( _pOwner!=rhs._pOwner)
			throw exDifferentMatrices();

		if (_pData!=rhs._pData)
			return false;
		
		return true;
	}

	bool operator<(const WindowIterator<T> & rhs) const
	{
		if ( _pOwner!=rhs._pOwner)
			throw exDifferentMatrices();

		return _pData<rhs._pData;
	}

	bool operator>(const WindowIterator<T> & rhs) const
	{
		if ( _pOwner!=rhs._pOwner)
			throw exDifferentMatrices();

		return _pData>rhs._pData;
	}
	
	bool operator!=(const WindowIterator<T> & rhs) const
	{
		if ( _pOwner!=rhs._pOwner)
			throw exDifferentMatrices();

		if (_pData!=rhs._pData)
			return true;
		
		return false;
	}
	
	void operator+=(int i)
	{
		_pData+=i;
		size_t nCol = _nCol + i;

		if (_pOwner->getColNr() <= nCol){
			_nCol = nCol % _pOwner->getColNr();
			_nRow += nCol / _pOwner->getColNr();
		}
	}

	void stepRow(int n)
	{
		_nRow+=n;
		_pData+=n*_powner->getColNr();
	}

	void stepCol(int n)
	{
		_nCol+=n;
		_pData+=n;
	}

	bool isValid() const
	{
		return (_pOwner!=NULL && _pData==NULL );
	}

	WindowIterator<T> operator[](int i) const
	{
		WindowIterator<T> iNew;
		iNew = *this;
		iNew+=i;

		return iNew;
	}

	
	int operator-(const WindowIterator & rhs) const
	{
		return (_pData - rhs._pData);
	}
	

	size_t getMatrixRowNr() const
	{
		return _pOwner->getRowNr();
	}

	size_t getMatrixColNr() const
	{
		return _pOwner->getColNr();
	}

	bool isInOutflowPos() const
	{
		size_t nMxRowNr = _pOwner->getRowNr();
		size_t nMxColNr = _pOwner->getColNr();
		
		eOutflowType OutFlowType = ModelParams::getModelParamsObj().getOutflowType();

		switch (OutFlowType) {
				case ofTopLeft:
					if ( _nCol==0 && _nRow==0 )
						return true;
				break;
				case ofTopRight:
					if ( _nCol==nMxColNr-1 && _nRow==0 )
						return true;
				break;
				case ofBottomLeft:
					if ( _nCol==0 && _nRow==nMxRowNr-1 )
						return true;
				break;
				case ofBottomRight:
					if ( _nCol==nMxColNr-1 && _nRow==nMxRowNr-1 )
						return true;
				break;
				case ofTopSide:
					if ( _nRow== 0 )
						return true;
				break;
				case ofRightSide:
					if ( _nCol==nMxColNr-1)
						return true;
				break;
				case ofBottomSide:
					if ( _nRow==nMxRowNr-1 )
						return true;
				break;
				case ofLeftSide:
					if ( _nCol==0)
						return true;

				break;
				case ofAllSides:
					if ( _nCol==0 || _nRow==0 || _nCol==nMxColNr-1 || _nRow==nMxRowNr-1 )
						return true;
				break;
				case ofTopAndBottomSide:
					
					if (_nRow==0 || _nRow==nMxRowNr-1 )
						return true;

				break;
				case ofLeftAndRightSide:
					if ( _nCol==0 || _nCol==nMxColNr-1 )
						return true;
				break;
				case ofTopAndMiddleOfBottom:
					if ( _nRow == 0 || 
						 (_nRow==nMxRowNr-1 && _nCol==(nMxColNr/2)))
						return true;
				case ofRightSideAndTopLeft:
					if ( _nCol == nMxColNr-1 || 
						 (_nRow==0 && _nCol==0))
						return true;
				break;
				case ofRightSideAndLeftMiddle:
					if ( _nCol == nMxColNr-1 || 
						 (_nRow==(nMxRowNr/2) && _nCol==0))
						return true;
				break;
			}
		
		return false;
	}

	bool isOnBound() const
	{
		return _nRow==0 || _nCol==0 || _nRow==_pOwner->getRowNr()-1 || _nCol==_pOwner->getColNr()-1;
	}

	void skipToChainCode( unsigned char nChainCode )
	{
		dCoords delta = getChainCodeDelta(nChainCode);
		
		_nRow+=delta._nDRow;
		_nCol+=delta._nDCol;
		
		_pData = &_pOwner->operator()(_nRow,_nCol);
	}

};

}

#endif