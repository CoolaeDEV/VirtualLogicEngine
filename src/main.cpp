
//Main file for the project. Testing etc.

#include <iostream>
#include "core/Simulation.hpp"

int main(){
	Circuit circuit(2, 1);
	
	Gate gate(1, Gate::types::BUFFER);
	
	Wire wire(1);
	
	circuit.addGate(gate);
	
	circuit.addWire(wire);
	
	circuit.connectWire(&wire, std::variant<Circuit*, Gate*>{&circuit}, std::variant<Circuit*, Gate*>{&gate});
	
	circuit.levelize();
	
	return 0;
}
