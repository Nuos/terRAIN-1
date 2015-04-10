#include "Simulation.h"

#include "OperationInterface.h"
#include "ParamNames.h"

namespace SIMULATION
{

Simulation::Simulation(ostream & os, ParamHandler & params):_os(os), _params(params)
{
	setOutputDirectory(params.paramAsString(OUTPUT_DIRECTORY).c_str());
}

void Simulation::printmx( const DblRasterMx & mx )
{

	size_t nRows = mx.getRowNr();
	size_t nCols = mx.getColNr();

	for (size_t i = 0 ; i < nRows ; i++){
		for (size_t j = 0 ; j < nCols ; j++) {
			// _os << std::setw(6)<<std::setprecision(3)<< mx(i,j) <<" ";
			_os << std::setw(6) << std::setprecision(4) << mx(i,j) <<" ";
			
		}
		_os << std::endl;		
	}
	_os << std::endl;
	_os << std::endl;
	
}

void Simulation::printmx( MultiflowDMatrix & mx )
{	
	size_t nRows = mx.getRowNr();
	size_t nCols = mx.getColNr();

	for (size_t i = 0 ; i < nRows ; i++){		
		
		_os << i << ". row:"<<std::endl;

		for (size_t j = 0 ; j < nCols ; j++) {
			_os << "[ ";
			DoubleChainCodeData & dccd = mx(i,j);
			for (unsigned char cc = 1; cc<10; cc++){
				
				if (cc==5)
					continue;

				_os << dccd.getByChainCode(cc) << " ";
			}
			
			_os << "]";
		}
		_os << std::endl;		
	}

	_os << std::endl;
	_os << std::endl;
	
}

};