#include "CircuitComponent.hpp"
#include <cassert>

void Wire::process(size_t circuitInputIndex) {
	if (source) {
		for (int i = 0; i < source->size(); i++) {
			if (std::holds_alternative<Gate>((*source)[i])) {
				Gate gate = std::get<Gate>((*source)[i]);
				data = gate.outputs;
			}
			else if (std::holds_alternative<Circuit>((*source)[i])) {
				Circuit circuit = std::get<Circuit>((*source)[i]);
				data = circuit.inputs[circuitInputIndex];
			}
		}
	}

}
// Gate class implementation
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

void Gate::setInput(size_t index, const DynamicBitset& input) {
	inputs[index] = input;
}

// Circuit class implementation
void Circuit::process() {

}

void Circuit::levelize() {
	for (Wire wire : wires) {
		for (int i = 0; i < wire.source->size(); i++) {
			if (std::holds_alternative<Gate>((*wire.source)[i])) {
				Gate gate = std::get<Gate>((*wire.source)[i]);
				wire.level = gate.level;
			}
			if (std::holds_alternative<Circuit>((*wire.source)[i])) {
				Circuit circuit = std::get<Circuit>((*wire.source)[i]);
				wire.level = circuit.level;
			}
		}
	}
	std::vector<int> inputLevels;
	for (Gate gate : gates) {
		for (Wire wire : gate.inputWires) {
			inputLevels.push_back(wire.level);
		}
		int maxValue = std::numeric_limits<int>::min();
		for (int num : inputLevels) {
			if (num > maxValue) {
				maxValue = num;
			}
		}
		int index = -1;
		for (size_t i = 0; i < gates.size(); i++) {
			if (&gates[i] == &gate) {
				index = static_cast<int>(i);
			}
		}
		gates[index].level = maxValue + 1;
	}


}

void Circuit::connectWire(Wire* wire, std::variant<Circuit, Gate> source, std::variant<Circuit, Gate> destination) {
	std::visit([&](auto&& arg) {  
		wire->source->emplace_back(arg);
	}, source);  
	
	std::visit([&](auto&& arg) {
		wire->destination->emplace_back(arg);
	}, destination);


	if (std::holds_alternative<Gate>(source)) {
		Gate gate = std::get<Gate>(source);
		auto it = std::find(gates.begin(), gates.end(), gate);
		if (it != gates.end()) {
			gates[std::distance(gates.begin(), it)].outputWires.emplace_back(*wire);
		}
	}

	if (std::holds_alternative<Gate>(destination)) {
		Gate gate = std::get<Gate>(destination);
		auto it = std::find(gates.begin(), gates.end(), gate);
		if (it != gates.end()) {
			gates[std::distance(gates.begin(), it)].inputWires.emplace_back(*wire);
		}
	}

	if (std::holds_alternative<Circuit>(source)) {
		Circuit circuit = std::get<Circuit>(source);
		auto it = std::find(subCircuits.begin(), subCircuits.end(), circuit);
		if (it != subCircuits.end()) {
			subCircuits[std::distance(subCircuits.begin(), it)]->inputs.emplace_back(wire->data);
		}
	}

	if (std::holds_alternative<Circuit>(destination)) {
		Circuit circuit = std::get<Circuit>(destination);
		auto it = std::find(subCircuits.begin(), subCircuits.end(), circuit);
		if (it != subCircuits.end()) {
			subCircuits[std::distance(subCircuits.begin(), it)]->outputs.emplace_back(wire->data);
		}
	}

}

void Circuit::addGate(Gate gate) {
	gates.push_back(gate);
}

void Circuit::addWire(Wire wire) {
	wires.push_back(wire);
}
void Circuit::addSubCircuit(Circuit* circuit) {
	subCircuits.push_back(circuit);
}