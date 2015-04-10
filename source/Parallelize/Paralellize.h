#ifndef PARALELLIZE_H
#define PARALELLIZE_H

#include <boost/thread/barrier.hpp>
#include <boost/thread/thread.hpp>
#include <vector>
#include <list>
#include "rastermatrix.h"
#include "paralellisable.h"
#include "accfluxdefs.h"
#include "SimpleMxOperations.h"
#include "StreamFuncDefs.h"
#include "longestFlowPathLengthDefs.h"
#include "MultiflowAccflux.h"
namespace TR
{

class UnaryMatrixFunc;
class BinaryMatrixFunc;
class SimpleMatrixFunc;
class UnaryWindowFunc;
class BinaryWindowFunc;
class MultiflowLDD;
class UnaryMLDDFunc;



class ThreadFunc 
{

private:
	paralellisable  * _pPar;
public:
	ThreadFunc( paralellisable & par): _pPar(&par){}
	
	void operator()(void)
	{
		_pPar->run();
	}

};


class Paralellize
{
public:
	typedef std::list<DblRasterMx> typeMxList;
private:

	std::vector<paralellisable *> _paralellData;
	unsigned int	_nThreads;
	
public:
	Paralellize( UnaryMatrixFunc & funcUnary, size_t nThreads );
	Paralellize( BinaryMatrixFunc & funcBinary, size_t nThreads );
	Paralellize( SimpleMatrixFunc & funcSimple, size_t nThreads );
	Paralellize( UnaryWindowFunc & funcWUnary, size_t nThreads );
	Paralellize( BinaryWindowFunc & funcWBinary, size_t nThreads );
	Paralellize( MultiflowLDD & funcMultiflowLDD, size_t nThreads );
	Paralellize( accfluxFuncOnLDD & funcAccflux, size_t nThreads );
	Paralellize( MultiflowAccflux & funcAccflux, size_t nThreads );
	Paralellize( mfLDDUpStreamTotal & funcStream, size_t nThreads );
	Paralellize( mfMLDDUpStreamTotal & funcStream, size_t nThreads );
	Paralellize( mfLDDUpStreamMin & funcStream, size_t nThreads );
	Paralellize( mfMLDDUpStreamMin & funcStream, size_t nThreads );
	Paralellize( mfLDDUpStreamMax & funcStream, size_t nThreads );
	Paralellize( mfMLDDUpStreamMax & funcStream, size_t nThreads );

	Paralellize( mfLDDDownStreamTotal & funcStream, size_t nThreads );
	Paralellize( mfMLDDDownStreamTotal & funcStream, size_t nThreads );
	Paralellize( funcLongestFlowPathLengthLDD & func, size_t nThreads );
	Paralellize( funcLongestFlowPathLengthMLDD & func, size_t nThreads );
	Paralellize( mfMLDDDownStreamMin & funcStream, size_t nThreads );
	Paralellize( mfMLDDDownStreamMax & funcStream, size_t nThreads );
	Paralellize( UnaryMLDDFunc & funcUnary, size_t nThreads );


	

	~Paralellize();
	void DecomposeMx( const DblRasterMx & src, typeMxList & listMx ) const;
	
protected:
	
	template<class TAccFluxFunc>
	void paralellizeAccFluxFunc( TAccFluxFunc & funcAccflux,size_t nThreads)
	{
		
		if (nThreads<2){
			funcAccflux.run();
			return;
		}
		
		typename TAccFluxFunc::iterator & iLDDBegin = funcAccflux.getLDDBegin();
		typename TAccFluxFunc::iterator & iLDDEnd	= funcAccflux.getLDDEnd();

		DblRasterMx & mxDepth = funcAccflux.getDepthMx();
		DblRasterMx & mxRet	  = funcAccflux.getRetMx();

		std::list<typename TAccFluxFunc::iterator> listIterators;
		std::list<DblRasterMx *> listRetMx;
		
		boost::thread_group		threads;

		int nData = iLDDEnd-iLDDBegin;

		if (nData<=0)
			return;

		size_t nCnt = nThreads;
		
		int nStep = static_cast<int>(nData/nCnt);

		_paralellData.resize(nCnt);
		
		listIterators.push_back(iLDDBegin);
		

		for (size_t i = 0; i < nCnt; i++){
			
			TAccFluxFunc * pFunc = static_cast<TAccFluxFunc *>(funcAccflux.clone());
			
			if ( i == nCnt-1 ) {
				pFunc->setLDDBegin(listIterators.back());
				pFunc->setLDDEnd(iLDDEnd);
			
			}else {
				pFunc->setLDDBegin(listIterators.back());
				iLDDBegin+=nStep;
				listIterators.push_back(iLDDBegin);
				pFunc->setLDDEnd(listIterators.back());

				DblRasterMx * pRet = new DblRasterMx(mxRet);

				pFunc->setRetMx(*pRet);
				listRetMx.push_back(pRet);

			}


			_paralellData[i] = pFunc;
			threads.create_thread(ThreadFunc(*pFunc));
		
		}

		threads.join_all();

		std::list<DblRasterMx *>::iterator iIter = listRetMx.begin();
		std::list<DblRasterMx *>::iterator iEnd = listRetMx.end();

		for (; iIter!=iEnd;++iIter){
			
			mfAdd addObj(**iIter,mxRet,mxRet,mfAdd::typeFunctor());

			Paralellize(addObj,nThreads);

			delete *iIter;
		}



	}
	
	template <class TStreamFunc>
	void paralellizeStreamFunc( TStreamFunc & funcStream,size_t nThreads)
	{
		if (nThreads<2){
			funcStream.run();
			return;
		}
		
		typename TStreamFunc::iteratorLDD & iLDDBegin = funcStream.getLDDBegin();
		typename TStreamFunc::iteratorLDD & iLDDEnd   = funcStream.getLDDEnd();
		
		DblRasterMx::iterator & iDepth = funcStream.getDepth();
		DblRasterMx::iterator & iRet   = funcStream.getRet();

		std::list<typename TStreamFunc::iteratorLDD> listLDDIterators;
		std::list<DblRasterMx::iterator>		   listMxIterators;
		
		boost::thread_group		threads;

		int nData = iLDDEnd-iLDDBegin;

		if (nData<=0)
			return;

		size_t nCnt = nThreads;
		
		int nStep = static_cast<int>(nData/nCnt);

		_paralellData.resize(nCnt);
		
		listLDDIterators.push_back(iLDDBegin);
		

		for (size_t i = 0; i < nCnt; i++){
			
			TStreamFunc * pFunc = static_cast<TStreamFunc *>(funcStream.clone());
			
			if ( i == nCnt-1 ) {
				pFunc->setLDDBegin(listLDDIterators.back());
				pFunc->setLDDEnd(iLDDEnd);
				pFunc->setDepth(iDepth);
				pFunc->setRet(iRet);
			
			}else {
				pFunc->setLDDBegin(listLDDIterators.back());
				iLDDBegin+=nStep;
				listLDDIterators.push_back(iLDDBegin);
				
				pFunc->setLDDEnd(listLDDIterators.back());
				listMxIterators.push_back(iDepth);
				pFunc->setDepth(listMxIterators.back());
				iDepth+=nStep;

				listMxIterators.push_back(iRet);
				pFunc->setRet(listMxIterators.back());
				iRet+=nStep;
			}


			_paralellData[i] = pFunc;
			threads.create_thread(ThreadFunc(*pFunc));
		
		}

		threads.join_all();
	}

	template <class TFunc>
	void paralellizeLongestFlowPathLengthFunc(TFunc & func,size_t nThreads)
	{
		if (nThreads<2){
			func.run();
			return;
		}
		
		typename TFunc::iteratorLDD & iLDDBegin = func.getLDDBegin();
		typename TFunc::iteratorLDD & iLDDEnd	= func.getLDDEnd();

		DblRasterMx & mxRet	  = func.getRetMx();

		std::list<typename TFunc::iteratorLDD> listIterators;
		std::list<DblRasterMx *> listRetMx;
		
		boost::thread_group		threads;

		int nData = iLDDEnd-iLDDBegin;

		if (nData<=0)
			return;

		size_t nCnt = nThreads;
		
		int nStep = static_cast<int>(nData/nCnt);

		_paralellData.resize(nCnt);
		
		listIterators.push_back(iLDDBegin);
		

		for (size_t i = 0; i < nCnt; i++){
			
			TFunc * pFunc = static_cast<TFunc *>(func.clone());
			
			if ( i == nCnt-1 ) {
				pFunc->setLDDBegin(listIterators.back());
				pFunc->setLDDEnd(iLDDEnd);
			
			}else {
				pFunc->setLDDBegin(listIterators.back());
				iLDDBegin+=nStep;
				listIterators.push_back(iLDDBegin);
				pFunc->setLDDEnd(listIterators.back());

				DblRasterMx * pRet = new DblRasterMx(mxRet);

				pFunc->setRetMx(*pRet);
				listRetMx.push_back(pRet);

			}


			_paralellData[i] = pFunc;
			threads.create_thread(ThreadFunc(*pFunc));
		
		}

		threads.join_all();
		
		std::list<DblRasterMx *>::iterator iIter = listRetMx.begin();
		std::list<DblRasterMx *>::iterator iEnd = listRetMx.end();

		for (; iIter!=iEnd;++iIter){
			
			mfMax maxObj(**iIter,mxRet,mxRet,mfMax::typeFunctor());

			Paralellize(maxObj,nThreads);

			delete *iIter;
		}
		
	}


};



}
#endif