#ifndef CHAINCODEDATA_H
#define CHAINCODEDATA_H

#include <ostream>
namespace TR
{

template<class T>
class ChainCodeData
{	
	
public:
	typedef T typeData;
protected:
	static typeData _Default;
	typeData _Data[8];
public:
	
	ChainCodeData()
	{
	}

	ChainCodeData(const ChainCodeData & src)
	{
		for (int i = 0; i < 8; i++)
			_Data[i] = src._Data[i];
	}

	ChainCodeData(const T & initValue)
	{
		for (int i = 0; i < 8; i++)
			_Data[i] = initValue;
	}

	static void setDefault(const T & def)
	{
		_Default = def;
	}
	
	static T getDefault()
	{
		return _Default;
	}

	T & getByChainCode( unsigned char nChainCode )
	{
		if (nChainCode<5)
			return _Data[nChainCode-1];
		else if (nChainCode>5 && nChainCode<10)
			return _Data[nChainCode-2];

		return _Default;
	}
	
	void setByChainCode( unsigned char nChainCode, const T & data)
	{
		if (nChainCode<5)
			_Data[nChainCode-1] = data;
		else if (nChainCode>5 && nChainCode<10)
			_Data[nChainCode-2] = data;
	}

	T & operator()( unsigned int n)
	{
		return _Data[n];
	}
	const T & operator()( unsigned int n ) const
	{
		return _Data[n];
	}

	ChainCodeData & operator=( const ChainCodeData & rhs )
	{
		
		for (int i = 0; i < 8; i++)
			_Data[i] = rhs._Data[i];

		return *this;
	}

	friend std::ostream & operator<<(std::ostream & os, ChainCodeData<T> & chd) 
	{
		os<<"[ ";

		for (int i = 0; i < 8; i++){
			os<< chd._Data[i];
			if (i<7)
				os<<",";
		}
		os<<" ]";
		
		return os;
	}

};

template<class T>
T ChainCodeData<T>::_Default;

}

#endif