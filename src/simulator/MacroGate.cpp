#include "headers/MacroGate.h"

MacroGate::MacroGate(const std::vector<Wire*>& inputs, const std::vector<Wire*>& outputs, Circuit* internalCircuit)
	: Gate(inputs, outputs), circuit(internalCircuit) {
}

void MacroGate::evaluate() {
	const auto& macroInputs = circuit->getInputWires();
	for (size_t i = 0; i < inputs.size(); i++) {
		macroInputs[i]->setValue(inputs[i]->getValue());
	}

	circuit->simulateTick();

	const auto& macroOutputs = circuit->getOutputWires();
	for (size_t i = 0; i < outputs.size(); ++i) {
		outputs[i]->setValue(macroOutputs[i]->getValue());
	}
}