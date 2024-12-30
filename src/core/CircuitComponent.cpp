#include "CircuitComponent.hpp"

void Wire::process() {
	if (destination) {
		Gate* gate = dynamic_cast<Gate*>(destination);
		if (gate) {
			gate->setInput(destinationInputIndex, data);
		}
	}
}

Gate::Gate(size_t outputSize, types gateType) 
    : outputs(outputSize) {
    Type = gateType;
    switch (this->Type) {
    case(BUFFER):
		this->inputs.resize(1);
		inputs[0].resize(1);
        break;
    case(AND || OR || OR || XOR || NOT || NAND || XOR || XNOR):
		this->inputs.resize(2);
		for (int i = 0; i < 2; i++) {
			inputs[i].resize(1);
		}
		break;
    }
}


static void pushToWire(Gate* gate) {
	for (Wire wire : gate->outputWires) {
		wire.data = gate->outputs;
	}
}

void Gate::process() {
    switch (Type) {
	case(BUFFER):
		outputs.set(0, inputs[0].get(0));
		pushToWire(this);
		break;
	case(AND):
		outputs.set(0, inputs[0].get(0) && inputs[1].get(0));
		pushToWire(this);
		break;
	case(OR):
		outputs.set(0, inputs[0].get(0) || inputs[1].get(0));
		pushToWire(this);
		break;
	case(NOT):
		outputs.set(0, !inputs[0].get(0));
		pushToWire(this);
		break;
	case(NAND):
		outputs.set(0, !(inputs[0].get(0) && inputs[1].get(0)));
		pushToWire(this);
		break;
	case(NOR):
		outputs.set(0, !(inputs[0].get(0) || inputs[1].get(0)));
		pushToWire(this);
		break;
	case(XOR):
		outputs.set(0, inputs[0].get(0) ^ inputs[1].get(0));
		pushToWire(this);
		break;
	case(XNOR):
		outputs.set(0, !(inputs[0].get(0) ^ inputs[1].get(0)));
		pushToWire(this);
		break;

    }
}

void Gate::addInput(size_t inputSize) {
	inputs.emplace_back(inputSize);
}
void Gate::setInput(size_t index, const DynamicBitset& input) {
	inputs[index] = input;
}

void Gate::connectInput(Wire& wire, size_t inputIndex) {
	wire.destinationInputIndex = inputIndex;
	wire.destination = this;
	inputWires.push_back(wire);
}
void Gate::connectOutput(Wire& wire) {
	outputWires.push_back(wire);
	wire.source = this;
}

void Circuit::process() {
	//need better Runtime Evaluation
	for (Wire wire : wires) {
		wire.process();
	}
	for (CircuitComponent* component : components) {
		component->process();
	}

}

void Circuit::buildLUT() {
	size_t inputCombination = 1ULL << static_cast<uint64_t>(inputSize);
	for (size_t i = 0; i < inputCombination; i++) {
		DynamicBitset input(inputSize);
		DynamicBitset output(outputSize);

		for (size_t j = 0; j < inputSize; j++) {
			input.set(j, (i >> j) & 1);
		}
		for (CircuitComponent* component : components) {
			if (auto gate = dynamic_cast<Gate*>(component)) {
				gate->inputs[0] = input;
				gate->process();
				output = gate->outputs;
			}
		}
		LookupTable[input] = output;
	}
}

DynamicBitset Circuit::evaluateLUT(const DynamicBitset& input) {
	if (LookupTable.find(input) != LookupTable.end()) {
		return LookupTable[input];
	}
}
void Circuit::addComponent(CircuitComponent* component) {
	components.push_back(component);
}

void Circuit::addWire(Wire wire) {
	wires.push_back(wire);
}