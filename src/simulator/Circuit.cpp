#include "headers/Circuit.h"
#include <iostream>

Wire* Circuit::createWire(Gate* from, int inputIndex, Gate* to) {
	Wire* wire = new Wire(from, to, inputIndex);

	wires.push_back(wire);
	return wire;
}

Gate* Circuit::createGate(GateType Gtype) {  
	Gate* gate = nullptr;

	switch (Gtype) {
	case GateType::AND:
		gate = new AndGate(gate->inputs, gate->output);
		break;
	case GateType::NOT:
		gate = new NotGate(gate->inputs[0], gate->output);
		break;
	case GateType::MacroGate:
		throw std::invalid_argument("Macro gates require a different constructor.");
	case GateType::BUFFER:

		gate = new BufferGate(gate->inputs[0], gate->output);
		break;
	case GateType::OR:

		gate = new ORGate(gate->inputs, gate->output);
		break;
	case GateType::NOR:

		gate = new NorGate(gate->inputs, gate->output);
		break;
	case GateType::NAND:

		gate = new NANDGate(gate->inputs, gate->output);
		break;
	case GateType::XOR:

		gate = new XORGate(gate->inputs, gate->output);
		break;
	case GateType::XNOR:

		gate = new XNORGate(gate->inputs, gate->output);
		break;
	default:
		throw std::invalid_argument("Unsupported gate type.");
	}

	gates.push_back(gate);

	for (Wire* input : gate->getInputs()) {
		input->attachGate(gate);
	}

	return gate;
}

void Circuit::markWireDirty(Wire* wire) {
	std::lock_guard<std::mutex> lock(dirtyMutex);
	if (!wire->isDirty()) {
		wire->markDirty();
		dirtyWires.push_back(wire);
	}
	else {
		dirtyWires.push_back(wire);
	}
}

void Circuit::simulateTick() {
	if (!levelsFinalized)
		throw std::runtime_error("Call finalizeLevels() first.");

	for (const auto& level : levelizedGates) {
		size_t gateCount = level.size();
		size_t chunkSize = std::max<size_t>(1, gateCount / threadCount);

		for (size_t i = 0; i < threadCount; ++i) {
			size_t start = i * chunkSize;
			size_t end = (i == threadCount - 1) ? gateCount : start + chunkSize;

			if (start >= gateCount) break;

			threadPool.enqueue([start, end, &level]() {
				for (size_t j = start; j < end; ++j) {
					level[j]->evaluate();
				}
			});
		}

		threadPool.waitAll();
	}
}

void Circuit::finalizeLevels() {
	// reset
	for (Gate* gate : gates) {
		gate->setLevel(-1);
	}

	std::cout << "[ CIRCUIT SIMULATION ] All Gates Resetted" << std::endl;

	// find the input gates (the ones with no driving gate)
	std::vector<Gate*> current;
	for (Gate* gate : gates) {
		bool isInputGate = true;
		for (Wire* in : gate->getInputs()) {
			bool hasDriver = false;
			for (Gate* g : gates) {
				if (g->getOutput() == in) {
					hasDriver = true;
					break;
				}
				if (hasDriver) break;
			}
			if (hasDriver) {
				isInputGate = false;
				break;
			}
		}
		if (isInputGate) {
			gate->setLevel(0);
			current.push_back(gate);
		}
		
	}
	std::cout << "[ CIRCUIT SIMULATION ] All Gates Have Found Input Gates" << std::endl;
	// assign levels stuff
	while (!current.empty()) {
		std::vector<Gate*> next;

		for (Gate* gate : current) {
			int lvl = gate->getLevel();

			for (Gate* sink : gate->getOutput()->getAttachedGates()) {
				int newLevel = lvl + 1;
				if (sink->getLevel() < newLevel) {
					sink->setLevel(newLevel);
					next.push_back(sink);
				}
			}
		}
		current = std::move(next);
		
	}
	std::cout << "[ CIRCUIT SIMULATION ] All Gates Assigned Levels" << std::endl;
	int maxLevel = 0;
	for (Gate* gate : gates) {
		maxLevel = std::max(maxLevel, gate->getLevel());
		
	}
	std::cout << "[ CIRCUIT SIMULATION ] Set All Gates Max Level" << std::endl;
	levelizedGates.clear();
	levelizedGates.resize(maxLevel + 1);

	for (Gate* gate : gates) {
		if (gate->getLevel() >= 0) {
			levelizedGates[gate->getLevel()].push_back(gate);
		}
	}

	for (auto* gate : levelizedGates[0]) {
		scheduleGate(gate, 0);
	}

	levelsFinalized = true;
}

void Circuit::scheduleGate(Gate* gate, uint32_t delay) {
	size_t targetTick = (currentTickIndex + delay) % tickMod;
	tickQueue[targetTick].emplace_back(gate, gate->getOutput()->getValue());
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

void Circuit::reset() {
	for (auto* gate : gates) {
		gate->reset();
	}
}