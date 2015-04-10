#ifndef SIMPLEMXOPERATIONS_H
#define SIMPLEMXOPERATIONS_H


#include "UnaryMatrixFuncT.h"
#include "BinaryMatrixFuncT.h"
#include "SimpleMatrixFunc.h"
#include "UnaryWindowFuncT.h"
#include "BinaryWindowFuncT.h"
#include "functors.h"
#include "UnaryMLDDFuncT.h"
#include "WindowOperationFunctors.h"
#include "RasterPositionFunctors.h"
#include "RasterPositionFuncT.h"
namespace TR
{
	typedef SimpleMatrixFuncT<FUNC::funcUniform> mfUniform;
	typedef SimpleMatrixFuncT<FUNC::funcSum> mfSum;

	typedef UnaryMatrixFuncT< FUNC::funcAbs > mfAbs;
	typedef UnaryMatrixFuncT< FUNC::funcSin > mfSin;
	typedef UnaryMatrixFuncT< FUNC::funcCos > mfCos;
	typedef UnaryMatrixFuncT< FUNC::funcTan > mfTan;
	typedef UnaryMatrixFuncT< FUNC::funcCTan > mfCTan;
	typedef UnaryMatrixFuncT< FUNC::funcArSin > mfArSin;
	typedef UnaryMatrixFuncT< FUNC::funcArCos > mfArCos;
	typedef UnaryMatrixFuncT< FUNC::funcArTan > mfArTan;
	typedef UnaryMatrixFuncT< FUNC::funcArCTan > mfArCTan;
	typedef UnaryMatrixFuncT< FUNC::funcLn > mfLn;
	typedef UnaryMatrixFuncT< FUNC::funcLog10 > mfLog10;
	typedef UnaryMatrixFuncT< FUNC::funcExp > mfExp;
	typedef UnaryMatrixFuncT< FUNC::funcCoverWithScalar > mfCovewWithScalar;

	typedef BinaryMatrixFuncT<FUNC::funcAdd> mfAdd;
	typedef BinaryMatrixFuncT<FUNC::funcSubt> mfSubt;
	typedef BinaryMatrixFuncT<FUNC::funcMul> mfMul;
	typedef BinaryMatrixFuncT<FUNC::funcFraction> mfFraction;
	typedef BinaryMatrixFuncT<FUNC::funcPower> mfPower;
	typedef BinaryMatrixFuncT<FUNC::funcCoverWithMatrix> mfCovewWithMatrix;
	typedef BinaryMatrixFuncT<FUNC::funcMax> mfMax;
	typedef BinaryMatrixFuncT<FUNC::funcMin> mfMin;

	typedef UnaryWindowFuncT<FUNC::funcSlope> mfSlope;
	typedef UnaryWindowFuncT<FUNC::funcSignStream> mfSignStream;
	typedef UnaryMLDDFuncT<FUNC::funcFindStreamMLDD> mfSignStreamMLDD;
	
	typedef UnaryWindowFuncT<FUNC::funcLDD> mfLDD;
	typedef UnaryWindowFuncT<FUNC::funcPIT> mfFindPITs;

	//typedef BinaryWindowFuncT<FUNC::funcFillPit> mfFillPits;
	
	typedef UnaryMLDDFuncT<FUNC::funcDiagonal> mfDiagonal;


	typedef UnaryWindowFuncT<FUNC::funcWindowAvarage> mfWindowAvarage;
	typedef UnaryWindowFuncT<FUNC::funcWindowMin> mfWindowMin;
	typedef UnaryWindowFuncT<FUNC::funcWindowMax> mfWindowMax;
	typedef UnaryWindowFuncT<FUNC::funcWindowTotal> mfWindowTotal;
	typedef UnaryWindowFuncT<FUNC::funcWindowVariance> mfWindowVariance;
	
	typedef RasterPositionFuncT<FUNC::funcDiagonalPosition> mfDiagonalPosition;
}


#endif