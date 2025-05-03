#include "headers/Gate.h"

Gate::Gate(const GateType GType, std::vector<Wire*>& inputs, std::vector<Wire*>& outputs) : inputs(inputs), outputs(outputs), type(GType){}

const std::vector<Wire*>& Gate::getInputs() const {
	return inputs;
}

const std::vector<Wire*>& Gate::getOutputs() const {
	return outputs;
}

void Gate::setLevel(int lvl) {
	level = lvl;
}

int Gate::getLevel() const{
	return level;
}

// Gate Implementation

AndGate::AndGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs) : Gate(GateType::AND, inputs, outputs) {}

void AndGate::evaluate() {
	bool result = true;
	for (Wire* wire : inputs) {
		if (wire->getValue() != Wire::HIGH) {
			result = false;
			break;
		}
	}

	outputs[0]->setValue(result ? Wire::HIGH : Wire::LOW);
}

NotGate::NotGate(Wire* input, std::vector<Wire*>& outputs)  
	: Gate(GateType::NOT, std::vector<Wire*>{input}, outputs) {  
}

void NotGate::evaluate() {
	if (inputs.empty() || outputs.empty()) return;

	// NOT gate logic: output is the inverse of the input  
	outputs[0]->setValue(inputs[0]->getValue() == Wire::Value::HIGH ? Wire::Value::LOW : Wire::Value::HIGH);
	outputs[0]->markDirty();
}