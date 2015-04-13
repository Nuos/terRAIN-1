#include "Simulation.h"

namespace SIMULATION
{

class SimpleRunoff : public Simulation
{
public:
	SimpleRunoff(ostream & os, ParamHandler & params);
	virtual bool run();
};

}