#include "SimulationRegistry.h"
#include "SimulationException.h"

namespace SIMULATION 
{

SimulationRegistry::SimulationRegistry() {}

SimulationRegistry::~SimulationRegistry() {
	
	map<string, Simulation* >::iterator it;

	for (it=_simulations.begin(); it!=_simulations.end(); ++it) {
		delete it->second;
	}
}

void SimulationRegistry::add(string name, Simulation * simulation) throw(...) {
	
	map<string, Simulation* >::iterator it = _simulations.find(name);
	if (it!=_simulations.end()) {
		string msg("This simulation name is already defined: ");
		msg+=name;
		delete simulation;
		throw SimulationException(msg.c_str());
	}
	_simulations[name] = simulation;
}

void SimulationRegistry::run(string name) throw(...) {
	
	map<string, Simulation* >::iterator it = _simulations.find(name);
	if (it==_simulations.end()) {
		string msg("This simulation name is not defined: ");
		msg+=name;
		throw SimulationException(msg.c_str());
	}
	it->second->run();
}

}