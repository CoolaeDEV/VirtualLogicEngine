#include "headers/MacroGate.h"

MacroGate::MacroGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs, Circuit* internalCircuit)
	: Gate(GateType::MacroGate, inputs, outputs), circuit(internalCircuit) {
}

void MacroGate::evaluate() {
    const auto& macroInputs = circuit->getInputWires();
    for (size_t i = 0; i < inputs.size(); i++) {
        macroInputs[i]->setValue(static_cast<Wire::Value>(inputs[i]->getValue()));
    }

    circuit->simulateTick();

    const auto& macroOutputs = circuit->getOutputWires();
    for (size_t i = 0; i < outputs.size(); ++i) {
        outputs[i]->setValue(static_cast<Wire::Value>(macroOutputs[i]->getValue()));
    }
}