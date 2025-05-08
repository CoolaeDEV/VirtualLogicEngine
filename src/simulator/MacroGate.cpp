#include "headers/MacroGate.h"

MacroGate::MacroGate(std::vector<Wire*>& inputs, Wire* output, Circuit* internalCircuit)
	: Gate(GateType::MacroGate, inputs, output), circuit(internalCircuit) {
}

void MacroGate::evaluate() {
    const auto& macroInputs = circuit->getInputWires();
    for (size_t i = 0; i < inputs.size(); i++) {
        macroInputs[i]->setValue(static_cast<Wire::Value>(inputs[i]->getValue()));
    }

    circuit->simulateTick();

    const auto& macroOutputs = circuit->getOutputWires();
    output->setValue(static_cast<Wire::Value>(macroOutputs[0]->getValue()));
}