#include "headers/Circuit.h"

Circuit::Circuit() : chunkSize(128) {}

Wire* Circuit::createWire() {
	Wire* wire = new Wire();

	wires.push_back(wire);
	return wire;
}

template<typename GateType, typename... Args>
GateType* Circuit::createGate(Args&&... args) {
	GateType* gate = new GateType(std::forward<Args>(args)...);
	gates.push_back(gate);

	for (Wire* input : gate->getInputs()) {
		input->attachGate(gate);
	}
	return gate;
}

template AndGate* Circuit::createGate<AndGate>(std::vector<Wire*>&&);
template NotGate* Circuit::createGate<NotGate>(Wire*&&, Wire*&&);
template MacroGate* Circuit::createGate<MacroGate>(std::vector<Wire*>&&, std::vector<Wire*>&&, Circuit*&&);

void Circuit::markWireDirty(Wire* wire) {
	std::lock_guard<std::mutex> lock(dirtyMutex);
	if (!wire->isDirty()) {
		wire->markDirty();
		dirtyWires.push_back(wire);
	}
}

void Circuit::simulateTick() {
	std::vector<Wire*> wiresToProcess;
	{
		std::lock_guard<std::mutex> lock(dirtyMutex);
		wiresToProcess.swap(dirtyWires);
	}
	if (wiresToProcess.empty()) return;

	std::vector<std::thread> workers;
	std::atomic<size_t> gatesProcessed(0);

	size_t numWires = wiresToProcess.size();
	size_t numChunkcs = (numWires + chunkSize - 1) / chunkSize;

	for (size_t chunk = 0; chunk < numChunkcs; chunk++) {
		size_t start = chunk * chunkSize;
		size_t end = std::min(start + chunkSize, numWires);

		workers.emplace_back(&Circuit::processChunk, this, start, end, std::ref(gatesProcessed));
	}

	for (auto& thread : workers) {
		thread.join();
	}

	for (Wire* wire : wires) {
		wire->clearDirty();
	}
}

void Circuit::processChunk(size_t start, size_t end, std::atomic<size_t>* gatesProcessed) {
	for (size_t i = start; i < end; i++) {
		Wire* wire = wires[i];
		if (wire->isDirty()) {
			const auto& attachedGates = wire->getAttachedGates();
			for (Gate* gate : attachedGates) {
				gate->evaluate();
				gatesProcessed++;
			}
		}
	}
}

const std::vector<Wire*>& Circuit::getInputWires() const {
	return inputWires;
}

const std::vector<Wire*>& Circuit::getOutputWires() const {
	return outputWires;
}

void Circuit::addInputWire(Wire* wire) {
	inputWires.push_back(wire);
}

void Circuit::addOutputWire(Wire* wire) {
	outputWires.push_back(wire);
}