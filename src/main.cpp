
//Main file for the project. Testing etc.

#include <iostream>
#include "core/Simulation.hpp"

int main(){
	Circuit circuit(2, 1);
	Gate gate(1, Gate::types::BUFFER);
	Gate gate2(1, Gate::types::AND);
	Wire wire(1);
	Wire wire2(1);
	circuit.addGate(gate);
	circuit.addWire(wire);
	circuit.addGate(gate2);
	circuit.addWire(wire2);
	circuit.connectWire(&wire, circuit, gate);
	circuit.connectWire(&wire2, gate, circuit);
	circuit.levelize();
	return 0;
}
