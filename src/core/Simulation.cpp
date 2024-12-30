// Initalization of the Simulation class and its methods
// The Simulation class handles simulating the LogicEngine.
// It handles Levelization, Event Queueing, compilation of sim, and more.

#include "Simulation.hpp"

Simulation::Simulation(SimulationMode initMode) : mode(initMode) {}

void Simulation::simulateRealtime() {
	for (Circuit* circuit : circuits) {
		circuit->process();
	}
}

void Simulation::buildCompiledLUT() {
	for (Circuit* circuit : circuits) {
		circuit->buildLUT();
	}
}

void Simulation::evaluteCompiledLUT() {
	for (Circuit* circuit : circuits) {
		DynamicBitset sampleInput(4);
		sampleInput.set(0, { 0b1 });
		sampleInput.set(1, { 0b0 });

		DynamicBitset result = circuit->evaluateLUT(sampleInput);
		std::cout << "Compiled LUT Result: " << result << "\n";
	}
}

void Simulation::tick() {
	if (mode == SimulationMode::Realtime) {
		simulateRealtime();
	}
	else {
		evaluteCompiledLUT();
	}
}
