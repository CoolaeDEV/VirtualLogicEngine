
//Main file for the project. Testing etc.

#include <iostream>
#include "core/Simulation.hpp"

int main(){
	Circuit circuit(1, 1);
	Gate* gate = new Gate(1, Gate::types::AND);

	Wire wire(1);
	Wire wire2(1);
	Wire wire3(1);

	wire.data.set(0, true);
	wire2.data.set(0, true);

	gate->connectInput(wire, 0);
	gate->connectInput(wire2, 1);
	gate->connectOutput(wire3);

	circuit.addComponent(gate);
	circuit.addWire(wire);
	circuit.addWire(wire2);
	circuit.addWire(wire3);
	
	Simulation sim(SimulationMode::Realtime);
	sim.addCircuit(&circuit);

	while (true) {
		sim.tick();
		std::cout << "Output of AND: " << wire3.data << "\n";
	}
	return 0;
}
