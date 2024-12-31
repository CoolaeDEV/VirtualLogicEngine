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

void Simulation::tick() {
	if (mode == SimulationMode::Realtime) {
		simulateRealtime();
	}
}

void Simulation::levelize() {
	for (Circuit* c : circuits) {
		c->levelize();
	}
}