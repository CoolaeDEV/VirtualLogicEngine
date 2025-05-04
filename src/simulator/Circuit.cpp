#include "headers/Circuit.h"

Wire* Circuit::createWire() {
	Wire* wire = new Wire();

	wires.push_back(wire);
	return wire;
}

Gate* Circuit::createGate(GateType Gtype, std::vector<Wire*>& inputs, std::vector<Wire*>& outputs) {  
	Gate* gate = nullptr;

	switch (Gtype) {
	case GateType::AND:
		if (outputs.size() != 1) {
			throw std::invalid_argument("AND gate requires exactly one output wire.");
		}
		gate = new AndGate(inputs, outputs);
		break;
	case GateType::NOT:
		if (inputs.size() != 1) {
			throw std::invalid_argument("NOT gate requires exactly one input wire.");
		}
		gate = new NotGate(inputs[0], outputs);
		break;
	case GateType::MacroGate:
		throw std::invalid_argument("Macro gates require a different constructor.");
	case GateType::BUFFER:
		if (inputs.size() != 1) {
			throw std::invalid_argument("BUFFER gates require a different constructor.");
		}
		gate = new BufferGate(inputs[0], outputs);
		break;
	case GateType::OR:
		if (inputs.size() != 1) {
			throw std::invalid_argument("OR gates require a different constructor.");
		}
		gate = new ORGate(inputs, outputs);
		break;
	case GateType::NOR:
		if (inputs.size() != 1) {
			throw std::invalid_argument("NOR gates require a different constructor.");
		}
		gate = new NorGate(inputs, outputs);
		break;
	case GateType::NAND:
		if (inputs.size() != 1) {
			throw std::invalid_argument("NAND gates require a different constructor.");
		}
		gate = new NANDGate(inputs, outputs);
		break;
	case GateType::XOR:
		if (inputs.size() != 1) {
			throw std::invalid_argument("XOR gates require a different constructor.");
		}
		gate = new XORGate(inputs, outputs);
		break;
	case GateType::XNOR:
		if (inputs.size() != 1) {
			throw std::invalid_argument("XOR gates require a different constructor.");
		}
		gate = new XNORGate(inputs, outputs);
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

void Circuit::processChunk(size_t start, size_t end, std::atomic<size_t>* gatesProcessed) {
	for (size_t i = start; i < end; i++) {
		Wire* wire = wires[i];
		if (wire->isDirty()) {
			const auto& attachedGates = wire->getAttachedGates();
			for (Gate* gate : attachedGates) {
				gate->evaluate();
				gatesProcessed->fetch_add(1, std::memory_order_relaxed);
			}
		}
	}
}

void Circuit::finalizeLevels() {
	// reset
	for (Gate* gate : gates) {
		gate->setLevel(-1);
	}

	// find the input gates (the ones with no driving gate)
	std::vector<Gate*> current;
	for (Gate* gate : gates) {
		bool isInputGate = true;
		for (Wire* in : gate->getInputs()) {
			bool hasDriver = false;
			for (Gate* g : gates) {
				for (Wire* out : g->getOutputs()) {
					if (out == in) {
						hasDriver = true;
						break;
					}
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

	// assign levels stuff
	while (!current.empty()) {
		std::vector<Gate*> next;

		for (Gate* gate : current) {
			int lvl = gate->getLevel();

			for (Wire* out : gate->getOutputs()) {
				for (Gate* sink : out->getAttachedGates()) {
					int newLevel = lvl + 1;
					if(sink->getLevel() < newLevel){
						sink->setLevel(newLevel);
						next.push_back(sink);
					}
				}
			}
		}
		current = std::move(next);
	}

	int maxLevel = 0;
	for (Gate* gate : gates) {
		maxLevel = std::max(maxLevel, gate->getLevel());
	}

	levelizedGates.clear();
	levelizedGates.resize(maxLevel + 1);

	for (Gate* gate : gates) {
		if (gate->getLevel() >= 0) {
			levelizedGates[gate->getLevel()].push_back(gate);
		}
	}
	levelsFinalized = true;
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