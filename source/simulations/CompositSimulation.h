#include "Simulation.h"
#include "OperationInterface.h"

namespace SIMULATION
{

class CompositSimulation : public Simulation
{
protected:

public:
	CompositSimulation(ostream & os, ParamHandler & params);
	virtual bool run();
};

}