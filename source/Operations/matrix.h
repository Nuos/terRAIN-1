#ifndef MATRIX_H
#define MATRIX_H

#include <algorithm>
#include <vector>

namespace TR
{

template  <class T>
class matrix
{
	friend class Paralellize;
public:
	typedef T typeData;
private:
	T * _pData;
	size_t _nRows;
	size_t _nCols;
	bool _bSuspendRelease;
public:
	matrix(): _pData(NULL), _nRows(0), _nCols(0), _bSuspendRelease(false) {}
	
	void release()
	{
		if (!_bSuspendRelease)
			delete [] _pData;
		
		_pData = NULL;
		_nRows = 0;
		_nCols = 0;
	}

	void init(size_t nRows, size_t nCols)
	{
		_bSuspendRelease = false;

		release();
		
		if ( nRows>0 && nCols>0){
			_pData = new T[nRows*nCols+1];
			_nRows = nRows;
			_nCols = nCols;
		}
	}

	void init(size_t nRows, size_t nCols, std::vector<T> & data)
	{
		if (data.size()!=nRows*nCols){
			//throw some exception
			return;
		}

		if (_nRows!=nRows || _nCols!=nCols)
			init(nRows,nCols);

		std::copy(data.begin(),data.end(),_pData);
	}

	matrix(size_t nRows, size_t nCols)
	{
		init(nRows,nCols);	
	}

	bool isValid() const
	{
		return _pData!=NULL;
	}

	size_t getRowNr() const
	{
		return _nRows;
	}
	size_t getColNr() const
	{
		return _nCols;
	}
	
	T * getData() const
	{
		return _pData;
	}

	matrix<T> & operator=(const matrix<T> & rhs)
	{
		
		if (this==&rhs)
			return *this;
		
		size_t nSize		= 0;
		T * pSrc			= rhs.getData();
		T * pCopy			= NULL;
		size_t nRows		= rhs.getRowNr();
		size_t nCols		= rhs.getColNr();
		_bSuspendRelease	= rhs._bSuspendRelease;

		if (rhs.isValid()){
			
			nSize	= nRows*nCols;

			if ( nRows!=_nRows || _nCols!=nCols ){
				
				release();

				_nRows	= nRows;
				_nCols	= nCols;
				_pData	= new T[nSize+1];
			}

			pCopy	= _pData;

			for (size_t i = 0; i < nSize; i++)
				*pCopy++=*pSrc++;
		}else
			release();

		return *this;

	}

	matrix(const matrix<T> & src): _pData(NULL), _nRows(0), _nCols(0)
	{
		(*this)=src;
	}

	~matrix()
	{
		release();
	}

	const T & operator()(size_t nRow, size_t nCol) const
	{
		return _pData[nRow*_nCols + nCol];
	}

	T & operator()(size_t nRow, size_t nCol)
	{
		return _pData[nRow*_nCols + nCol];
	}

	size_t getIndexOfPos(size_t nRow, size_t nCol) const
	{
		return nRow*_nCols + nCol;
	}
	
	void fill(const T & data)
	{
		if (isValid()){
			T * iBegin = _pData;
			std::fill_n(iBegin,_nRows*_nCols,data);
		}
	}
	
};

}

#endif MATRIX_H