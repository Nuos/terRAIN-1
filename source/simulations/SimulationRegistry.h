#ifndef SIMULATION_REGISTRY_H
#define SIMULATION_REGISTRY_H

#include <map>
#include <string>
#include "Simulation.h"

using namespace std;

namespace SIMULATION 
{
class SimulationRegistry
{
private:
	map<string, Simulation* > _simulations;
public:
	
	SimulationRegistry();
	~SimulationRegistry();

	void add(string name, Simulation * simulation);
	void run(string name);
};


}

#endif