#ifndef RASTERMATRIX_H
#define RASTERMATRIX_H

#include "defs.h"
#include "matrix.h"
#include "WindowIterator.h"
#include "DoubleUtil.h"
#include "ModelParams.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <math.h>
#include <string>

namespace TR
{

template <class T>
class rastermatrix
{
public:
	typedef T typeData;
	friend class Paralellize;
	typedef WindowIterator< rastermatrix<T> > iterator;
private:
	matrix<T> _mx;
	eOrigo _origo;
	double _dblPixelSize;

public:
	rastermatrix():_origo(origoBottomLeft),_dblPixelSize(0.0) {};

	void init( size_t nRows, size_t nCols, double dblPixelSize, eOrigo origo, const T & init)
	{
		_mx.init(nRows,nCols);

		_dblPixelSize	= dblPixelSize;
		_origo			= origo;

		if (_mx.isValid())
			_mx.fill(init);

	}

	rastermatrix( size_t nRows, size_t nCols, double dblPixelSize, eOrigo origo, const T & init)
	{
		this->init(nRows,nCols,dblPixelSize,origo,init);
	}

	void fill(const T & data)
	{
		_mx.fill(data);
	}

	const T & operator()(size_t nRow, size_t nCol) const
	{
		return _mx(nRow, nCol);
	}

	T & operator()(size_t nRow, size_t nCol)
	{
		return _mx(nRow, nCol);
	}

	size_t getIndexOfPos(size_t nRow, size_t nCol) const
	{
		return _mx.getIndexOfPos(nRow,nCol);
	}

	size_t getRowNr() const
	{
		return _mx.getRowNr();
	}

	size_t getColNr() const
	{
		return _mx.getColNr();
	}

	double getPixelSize() const
	{
		return _dblPixelSize;
	}

	eOrigo getOrigo() const
	{
		return _origo;
	}

	bool isValid() const
	{
		return _mx.isValid();
	}

	T * getData() const
	{
		return _mx.getData();
	}
	
	template<class TMatrix>
	void initlike( const TMatrix & src )
	{
		_dblPixelSize	= src.getPixelSize();
		_origo			= src.getOrigo();

		size_t nRows = src.getRowNr();
		size_t nCols = src.getColNr();

		if (_mx.getRowNr()==nRows && _mx.getColNr()==nCols)
			return;

		_mx.init(nRows,nCols);
	}
	
	iterator begin() const
	{		
		return iterator(const_cast<rastermatrix<T>*>(this),_mx.getData(),0,0);
	}

	iterator end() const
	{
		return iterator(const_cast<rastermatrix<T>*>(this),_mx.getData() + getRowNr()*getColNr(),getRowNr(),0);
	}

	iterator getIteratorAt(size_t nRow, size_t nCol)
	{
		iterator iIter;
		return iterator(const_cast<rastermatrix<T>*>(this),&_mx(nRow,nCol),nRow,nCol);		
	}

	void writeToColFormat(std::ostream & os)
	{
		double dblInit = _dblPixelSize*0.5;

		double dblRowPos = dblInit;
		double dblColPos = dblInit;

		size_t nRowNr = _mx.getRowNr();
		size_t nColNr = _mx.getColNr();

		size_t i = 0;
		size_t j = 0;

		T * pData = getData();

		for ( i = 0 ; i < nRowNr; i++, dblColPos = dblInit){
			
			for ( j = 0; j < nColNr; j++ ){
				os << std::setw(13)<< dblColPos;
				os << std::setw(13)<< dblRowPos;
				os << std::setw(13)<< *pData;
				os << std::endl;
				++pData;
				dblColPos+=_dblPixelSize;
			}

			dblRowPos+=_dblPixelSize;	
		}
	}

	void readFromColFormat(std::istream & is)
	{
		
		size_t nRowNr = 0;
		size_t nColNr = 0;
		bool bColNrIsInited = false;

		double dblFirstRowPos = 0.0;
		double dblFirstColPos = 0.0;
		double dblTmp		  = 0.0;

		std::vector<T> arrPixels;

		if (!is.eof()){

			nColNr = 1;
			is >> dblFirstColPos;
			is >> dblFirstRowPos;
			is >> dblTmp;

			arrPixels.push_back(dblTmp);
		}
		
		//read col pos:
		//read it befor the loop, to check if file is not at the end
		is >> dblTmp;
		
		while (!is.eof()) {
			//read row pos:
			is >> dblTmp;
			if (!bColNrIsInited && DoubleUtil::cmpD6(dblTmp,dblFirstRowPos)){
				++nColNr;	
			}

			//read pixel value:
			is >> dblTmp;
			arrPixels.push_back(dblTmp);
			// read col pos:
			// read it at the and of the loop to check if file contains only whitespaces 
			is >> dblTmp;
		}
		
		if (nColNr==0)
			return;

		nRowNr = arrPixels.size()/nColNr;

		_mx.init(nRowNr,nColNr,arrPixels);

		_dblPixelSize = fabs(dblFirstColPos)*2;
	}

	void writeToAsc(std::ostream & os, bool removeOutflowSides)
	{
		
		size_t nRowNr = _mx.getRowNr();
		size_t nColNr = _mx.getColNr();
		size_t i	  = 0;
		size_t j	  = 0;
		
		size_t startRowIndex = 0;
		size_t endRowIndex = nRowNr;
		size_t startColIndex = 0;
		size_t endColIndex = nColNr;

		if (removeOutflowSides) {
			eOutflowType outflowType = ModelParams::getModelParamsObj().getOutflowType();

			switch (outflowType) {
				case ofAllSides:
					startRowIndex++;
					endRowIndex--;
					startColIndex++;
					endColIndex--;
				break;
				case ofBottomSide:
					endRowIndex--;
				break;
				case ofTopSide:
				case ofTopAndMiddleOfBottom:
					startRowIndex++;
				break;
				case ofLeftSide:
					startColIndex++;
				break;
				case ofRightSide:
					endColIndex--;
				break;
				case ofLeftAndRightSide:
					startColIndex++;
					endColIndex--;
				break;
				case ofTopAndBottomSide:
					startRowIndex++;
					endRowIndex--;
				break;
			}
	
		}
		char buffer[256];
		for ( i = startRowIndex; i < endRowIndex; i++){
			for ( j = startColIndex; j < endColIndex; j++ ) {
				sprintf(buffer, "%f ", _mx(i,j));
				os << buffer;
			}
			os<<std::endl;
		}
	}

	void readFromAsc(std::istream & is, double dblPixelSize)
	{
		size_t nColNr = 0;
		size_t nRowNr = 0;
		
		std::string strTmp;

		bool bColNrIsInited = false;

		std::vector<T> arrPixels;

		double dblTmp = 0;

		is >> dblTmp;

		while (!is.eof()){
			
			if (!bColNrIsInited){
				++nColNr;
				if (is.peek()=='\n')
					bColNrIsInited=true;
			}
			
			arrPixels.push_back(dblTmp);

			is >> dblTmp;
		}

		if (nColNr == 0)
			return;

		nRowNr = arrPixels.size()/nColNr;

		_mx.init(nRowNr,nColNr,arrPixels);

		_dblPixelSize = dblPixelSize;
		

	}
};


typedef rastermatrix<double> DblRasterMx;
typedef rastermatrix<int> IntRasterMx;

}
#endif RASTERMATRIX_H