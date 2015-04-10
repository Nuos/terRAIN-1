#include "Paralellize.h"
#include "UnaryMatrixFunc.h"
#include "UnaryWindowFunc.h"
#include "BinaryMatrixFunc.h"
#include "BinaryWindowFunc.h"
#include "SimpleMatrixFunc.h"
#include "exParalellize.h"
#include "MultiflowLDD.h"
#include "accfluxFuncT.h"
#include "UnaryMLDDFunc.h"
#include <list>

namespace TR
{

Paralellize::Paralellize( funcLongestFlowPathLengthLDD & func, size_t nThreads )
{
	paralellizeLongestFlowPathLengthFunc(func,nThreads);
}

Paralellize::Paralellize( funcLongestFlowPathLengthMLDD & func, size_t nThreads )
{
	paralellizeLongestFlowPathLengthFunc(func,nThreads);
}

Paralellize::Paralellize( accfluxFuncOnLDD & funcAccflux, size_t nThreads )
{
	paralellizeAccFluxFunc(funcAccflux,nThreads);
}

Paralellize::Paralellize( MultiflowAccflux & funcAccflux, size_t nThreads )
{
	paralellizeAccFluxFunc(funcAccflux,nThreads);
}

Paralellize::Paralellize( mfLDDUpStreamTotal & funcStream, size_t nThreads )
{
	paralellizeStreamFunc(funcStream,nThreads);
}

Paralellize::Paralellize( mfLDDUpStreamMin & funcStream, size_t nThreads )
{
	paralellizeStreamFunc(funcStream,nThreads);
}

Paralellize::Paralellize( mfMLDDUpStreamMin & funcStream, size_t nThreads )
{
	paralellizeStreamFunc(funcStream,nThreads);
}

Paralellize::Paralellize( mfLDDUpStreamMax & funcStream, size_t nThreads )
{
	paralellizeStreamFunc(funcStream,nThreads);
}

Paralellize::Paralellize( mfMLDDUpStreamMax & funcStream, size_t nThreads )
{
	paralellizeStreamFunc(funcStream,nThreads);
}

Paralellize::Paralellize( mfMLDDUpStreamTotal & funcStream, size_t nThreads )
{
	paralellizeStreamFunc(funcStream,nThreads);
}

Paralellize::Paralellize( mfLDDDownStreamTotal & funcStream, size_t nThreads )
{
	paralellizeStreamFunc(funcStream,nThreads);
}

Paralellize::Paralellize( mfMLDDDownStreamTotal & funcStream, size_t nThreads )
{
	paralellizeStreamFunc(funcStream,nThreads);
}

Paralellize::Paralellize( mfMLDDDownStreamMin & funcStream, size_t nThreads )
{
	paralellizeStreamFunc(funcStream,nThreads);
}

Paralellize::Paralellize( mfMLDDDownStreamMax & funcStream, size_t nThreads )
{
	paralellizeStreamFunc(funcStream,nThreads);
}

Paralellize::Paralellize( UnaryMatrixFunc & funcUnary, unsigned int nThreads ): _nThreads(nThreads)
{
	if (nThreads<2){
		funcUnary.run();
		return;
	}

	typeMxList				listOpComps;
	typeMxList				listRetComps;
	boost::thread_group		threads;
	typeMxList::iterator	iOps;
	typeMxList::iterator	iRets;	
	size_t					i = 0;
	DblRasterMx & refOp		= funcUnary.getOp();
	DblRasterMx & refRet	= funcUnary.getRet();

	refRet.initlike(refOp);

	DecomposeMx(refOp,listOpComps);
	DecomposeMx(refRet,listRetComps);

	size_t nCnt = listOpComps.size();

	if (nCnt!=listRetComps.size() || nCnt==0){
		throw exParalellize();
	}

	_paralellData.resize(nCnt);

	for (iOps = listOpComps.begin(), iRets = listRetComps.begin(); i < nCnt; i++, ++iOps, ++iRets){
		
		UnaryMatrixFunc * pMxFunc = static_cast<UnaryMatrixFunc *>(funcUnary.clone());

		pMxFunc->setOp(*iOps);
		pMxFunc->setRet(*iRets);

		_paralellData[i] = pMxFunc;
		threads.create_thread(ThreadFunc(*pMxFunc));
	
	}

	threads.join_all();
}

Paralellize::Paralellize( BinaryWindowFunc & funcWBinary, size_t nThreads )
{
	
	
	if (nThreads<2){
		funcWBinary.run();
		return;
	}

	DblRasterMx::iterator  iOp1		= funcWBinary.getOp1Begin();
	DblRasterMx::iterator  iOp1End	= funcWBinary.getOp1End();
	DblRasterMx::iterator  iOp2		= funcWBinary.getOp2Begin();
	DblRasterMx::iterator  iRet		= funcWBinary.getRetIterator();
	
	std::list<DblRasterMx::iterator> listOp1Iterators;
	std::list<DblRasterMx::iterator> listOp2Iterators;
	
	boost::thread_group		threads;

	int nData = iOp1End-iOp1;

	if (nData<=0)
		return;

	size_t nCnt = nThreads;
	
	int nStep = static_cast<int>(nData/nCnt);

	_paralellData.resize(nCnt);
	
	listOp1Iterators.push_back(iOp1);
	

	for (size_t i = 0; i < nCnt; i++){
		
		BinaryWindowFunc * pMxFunc = static_cast<BinaryWindowFunc *>(funcWBinary.clone());
		
		if ( i == nCnt-1 ) {
			pMxFunc->setOp1Begin(listOp1Iterators.back());
			pMxFunc->setOp1End(iOp1End);
			pMxFunc->setOp2Begin(iOp2);
			pMxFunc->setRetIterator(iRet);
		}else {
			pMxFunc->setOp1Begin(listOp1Iterators.back());
			listOp1Iterators.push_back(iRet);
			pMxFunc->setRetIterator(listOp1Iterators.back());
			iOp1+=nStep;
			listOp1Iterators.push_back(iOp1);
			pMxFunc->setOp1End(listOp1Iterators.back());
			iRet+=nStep;

			listOp2Iterators.push_back(iOp2);
			pMxFunc->setOp2Begin(listOp2Iterators.back());
			iOp2+=nStep;

		}


		_paralellData[i] = pMxFunc;
		threads.create_thread(ThreadFunc(*pMxFunc));
	
	}

	threads.join_all();

	
}

Paralellize::Paralellize( UnaryWindowFunc & funcWUnary, size_t nThreads )
{
	
	if (nThreads<2){
		funcWUnary.run();
		return;
	}

	DblRasterMx::iterator  iOp		= funcWUnary.getOpBegin();
	DblRasterMx::iterator  iOpEnd	= funcWUnary.getOpEnd();
	DblRasterMx::iterator  iRet	= funcWUnary.getRetIterator();
	
	std::list<DblRasterMx::iterator> listIterators;
	
	boost::thread_group		threads;

	int nData = iOpEnd-iOp;

	if (nData<=0)
		return;

	size_t nCnt = nThreads;
	
	int nStep = static_cast<int>(nData/nCnt);

	_paralellData.resize(nCnt);
	
	listIterators.push_back(iOp);

	for (size_t i = 0; i < nCnt; i++){
		
		UnaryWindowFunc * pMxFunc = static_cast<UnaryWindowFunc *>(funcWUnary.clone());
		/*
		if ( i == nCnt-1 ) {
			pMxFunc->setOpBegin(listIterators.back());
			pMxFunc->setOpEnd(iOpEnd);
			pMxFunc->setRetIterator(iRet);
		}else {
			pMxFunc->setOpBegin(listIterators.back());
			listIterators.push_back(iRet);
			pMxFunc->setRetIterator(listIterators.back());
			iOp+=nStep;
			listIterators.push_back(iOp);
			pMxFunc->setOpEnd(listIterators.back());
			iRet+=nStep;
		}
		*/

		if ( i == nCnt-1 ) {
			pMxFunc->setOpBegin(iOp);
			pMxFunc->setOpEnd(iOpEnd);
			pMxFunc->setRetIterator(iRet);
		}else {
			pMxFunc->setOpBegin(iOp);
			iOp+=nStep;
			listIterators.push_back(iOp);
			pMxFunc->setOpEnd(iOp);
			pMxFunc->setRetIterator(iRet);
			iRet+=nStep;
		}

		_paralellData[i] = pMxFunc;
		threads.create_thread(ThreadFunc(*pMxFunc));
	
	}

	threads.join_all();

}

Paralellize::Paralellize( MultiflowLDD & funcMultiflowLDD, size_t nThreads )
{
	if (nThreads<2){
		funcMultiflowLDD.run();
		return;
	}

	DblRasterMx::iterator		iOp		= funcMultiflowLDD.getOpBegin();
	DblRasterMx::iterator		iOpEnd	= funcMultiflowLDD.getOpEnd();
	MultiflowDMatrix::iterator  iRet	= funcMultiflowLDD.getRetIterator();
	
	std::list<DblRasterMx::iterator>		listOpIterators;
	std::list<MultiflowDMatrix::iterator>	listRetIterators;
	
	boost::thread_group		threads;

	int nData = iOpEnd-iOp;

	if (nData<=0)
		return;

	size_t nCnt = nThreads;
	
	int nStep = static_cast<int>(nData/nCnt);

	_paralellData.resize(nCnt);
	
	listOpIterators.push_back(iOp);

	for (size_t i = 0; i < nCnt; i++){
		
		MultiflowLDD * pFunc = static_cast<MultiflowLDD *>(funcMultiflowLDD.clone());
		
		if ( i == nCnt-1 ) {
			pFunc->setOpBegin(listOpIterators.back());
			pFunc->setOpEnd(iOpEnd);
			pFunc->setRetIterator(iRet);
		}else {
			pFunc->setOpBegin(listOpIterators.back());
			listRetIterators.push_back(iRet);
			pFunc->setRetIterator(listRetIterators.back());
			iOp+=nStep;
			listOpIterators.push_back(iOp);
			pFunc->setOpEnd(listOpIterators.back());
			iRet+=nStep;
		}

		_paralellData[i] = pFunc;
		threads.create_thread(ThreadFunc(*pFunc));
	
	}

	threads.join_all();

}

Paralellize::Paralellize( BinaryMatrixFunc & funcBinary, unsigned int nThreads ): _nThreads(nThreads)
{
	
	if (nThreads<2){
		funcBinary.run();
		return;
	}
	
	typeMxList				listOp1Comps;
	typeMxList				listOp2Comps;
	typeMxList				listRetComps;
	boost::thread_group		threads;
	typeMxList::iterator	iOps1;
	typeMxList::iterator	iOps2;
	typeMxList::iterator	iRets;	
	size_t					i = 0;
	DblRasterMx & refOp1	= funcBinary.getOp1();
	DblRasterMx & refOp2	= funcBinary.getOp2();
	DblRasterMx & refRet	= funcBinary.getRet();

	refRet.initlike(refOp1);

	DecomposeMx(refOp1,listOp1Comps);
	DecomposeMx(refOp2,listOp2Comps);
	DecomposeMx(refRet,listRetComps);

	size_t nCnt = listOp1Comps.size();

	if (nCnt!=listRetComps.size() || nCnt!=listOp2Comps.size() || nCnt==0){
		throw exParalellize();
	}

	_paralellData.resize(nCnt);

	for (iOps1 = listOp1Comps.begin(),iOps2 = listOp2Comps.begin(), iRets = listRetComps.begin(); i < nCnt; i++, ++iOps1,++iOps2, ++iRets){
		
		BinaryMatrixFunc * pMxFunc = static_cast<BinaryMatrixFunc *>(funcBinary.clone());

		pMxFunc->setOp1(*iOps1);
		pMxFunc->setOp2(*iOps2);
		pMxFunc->setRet(*iRets);

		_paralellData[i] = pMxFunc;
		threads.create_thread(ThreadFunc(*pMxFunc));
	
	}

	threads.join_all();
}

Paralellize::Paralellize( SimpleMatrixFunc & funcSimple, size_t nThreads )
{
	if (nThreads<2){
		funcSimple.run();
		return;
	}

	typeMxList				listRetComps;
	boost::thread_group		threads;
	typeMxList::iterator	iRets;	
	size_t					i = 0;
	
	DblRasterMx & refRet	= funcSimple.getRet();

	DecomposeMx(refRet,listRetComps);

	size_t nCnt = listRetComps.size();

	_paralellData.resize(nCnt);

	for (iRets = listRetComps.begin(); i < nCnt; i++,++iRets){
		
		SimpleMatrixFunc * pMxFunc = static_cast<SimpleMatrixFunc *>(funcSimple.clone());

		pMxFunc->setRet(*iRets);

		_paralellData[i] = pMxFunc;
		threads.create_thread(ThreadFunc(*pMxFunc));
	
	}

	threads.join_all();
}

void Paralellize::DecomposeMx( const DblRasterMx & src, typeMxList & listMx  ) const
{
	if ( _nThreads < 1 || !src.isValid()){
		throw exParalellize();
	}

	eOrigo origo		= src.getOrigo();
	double dblPixelSize = src.getPixelSize();
	size_t nRows		= src.getRowNr();
	size_t nCols		= src.getColNr();
	double * pData		= src.getData();
	
	DblRasterMx	mxEmpty;
	
	size_t nParts = _nThreads > nRows ? nRows : _nThreads;

	if (nParts==0){
		//throw exception
		return;
	}
	size_t nNewRows = nRows / nParts;
	size_t nOffset  = nNewRows * nCols;
	

	for ( size_t i = 0 ; i<nParts ; i++, pData+=nOffset){
		
		listMx.push_back(mxEmpty);

		DblRasterMx & refData = listMx.back(); 

		refData._dblPixelSize = dblPixelSize;
		refData._origo		  = origo;
		
		refData._mx._pData = pData;
		refData._mx._nCols = nCols;
		refData._mx._nRows = i<(nParts-1) ? nNewRows : nRows - nNewRows*i;
		refData._mx._bSuspendRelease = true;

	}
}


Paralellize::~Paralellize()
{
	std::vector<paralellisable *>::iterator	iIter;

	for (iIter = _paralellData.begin(); iIter!=_paralellData.end(); ++iIter)
		delete *iIter;

}

Paralellize::Paralellize( UnaryMLDDFunc & funcUnary, size_t nThreads )
{
	
	if (nThreads<2){
		funcUnary.run();
		return;
	}

	MultiflowDMatrix::iterator	iOp		= funcUnary.getOpBegin();
	MultiflowDMatrix::iterator	iOpEnd	= funcUnary.getOpEnd();
	DblRasterMx::iterator  iRet	= funcUnary.getRetIterator();
	
	std::list<MultiflowDMatrix::iterator>		listOpIterators;
	std::list<DblRasterMx::iterator>	listRetIterators;
	
	boost::thread_group		threads;

	int nData = iOpEnd-iOp;

	if (nData<=0)
		return;

	size_t nCnt = nThreads;
	
	int nStep = static_cast<int>(nData/nCnt);

	_paralellData.resize(nCnt);
	
	listOpIterators.push_back(iOp);

	for (size_t i = 0; i < nCnt; i++){
		
		UnaryMLDDFunc * pFunc = static_cast<UnaryMLDDFunc *>(funcUnary.clone());
		
		if ( i == nCnt-1 ) {
			pFunc->setOpBegin(listOpIterators.back());
			pFunc->setOpEnd(iOpEnd);
			pFunc->setRetIterator(iRet);
		}else {
			pFunc->setOpBegin(listOpIterators.back());
			listRetIterators.push_back(iRet);
			pFunc->setRetIterator(listRetIterators.back());
			iOp+=nStep;
			listOpIterators.push_back(iOp);
			pFunc->setOpEnd(listOpIterators.back());
			iRet+=nStep;
		}

		_paralellData[i] = pFunc;
		threads.create_thread(ThreadFunc(*pFunc));
	
	}

	threads.join_all();
}



}