#include "headers/Gate.h"

Gate::Gate(const std::vector<Wire*>& inputs, const std::vector<Wire*>& outputs)
	: inputs(inputs), outputs(outputs) {
}

const std::vector<Wire*>& Gate::getInputs() const {
	return inputs;
}

const std::vector<Wire*>& Gate::getOutputs() const {
	return outputs;
}

// Gate Implementation

AndGate::AndGate(const std::vector<Wire*>& inputs, Wire* output) : Gate(inputs, { output }) {}

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

NotGate::NotGate(Wire* input, Wire* output) : Gate({ input }, { output }) {}

void NotGate::evaluate() {
	Wire::Value inputVal = inputs[0]->getValue();
	Wire::Value outputVal = (inputVal == Wire::LOW) ? Wire::HIGH : Wire::LOW;
	outputs[0]->setValue(outputVal);
}