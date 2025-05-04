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

void Gate::reset() {
	// potential error :/
	for (auto* output : outputs) {
		output = nullptr;
	}
}


// Gate Implementation



// AND
AndGate::AndGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs) : Gate(GateType::AND, inputs, outputs) {}

void AndGate::evaluate() {
	if (inputs.empty() || outputs.empty()) return;

	outputs[0]->setValue((inputs[0]->getValue() && inputs[1]->getValue() == Wire::HIGH) ? Wire::HIGH : Wire::LOW);
}

// NAND
NANDGate::NANDGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs) : Gate(GateType::AND, inputs, outputs) {}

void NANDGate::evaluate() {
	if (inputs.empty() || outputs.empty()) return;

	outputs[0]->setValue(!(inputs[0]->getValue() && inputs[1]->getValue() == Wire::HIGH) ? Wire::HIGH : Wire::LOW);
	outputs[0]->markDirty();
}

// NOT
NotGate::NotGate(Wire* input, std::vector<Wire*>& outputs) : Gate(GateType::NOT, std::vector<Wire*>{input}, outputs) {}

void NotGate::evaluate() {
	if (inputs.empty() || outputs.empty()) return;

	outputs[0]->setValue(inputs[0]->getValue() == Wire::Value::HIGH ? Wire::Value::LOW : Wire::Value::HIGH);
	outputs[0]->markDirty();
}

// BUFFER
BufferGate::BufferGate(Wire* input, std::vector<Wire*>& outputs) : Gate(GateType::BUFFER, std::vector<Wire*>{input}, outputs) {}

void BufferGate::evaluate() {
	if (inputs.empty() || outputs.empty()) return;

	outputs[0]->setValue(inputs[0]->getValue() == Wire::Value::HIGH ? Wire::Value::HIGH : Wire::Value::LOW);
	outputs[0]->markDirty();
}

// OR
ORGate::ORGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs) : Gate(GateType::OR, inputs, outputs){}

void ORGate::evaluate() {
	if (inputs.empty() || outputs.empty()) return;
	
	outputs[0]->setValue((inputs[0]->getValue() || inputs[1]->getValue()) == Wire::Value::HIGH ? Wire::Value::HIGH : Wire::Value::LOW);
	outputs[0]->markDirty();
}

// NOR
NorGate::NorGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs) : Gate(GateType::NOR, inputs, outputs){}

void NorGate::evaluate() {
	if (inputs.empty() || outputs.empty()) return;

	outputs[0]->setValue(!(inputs[0]->getValue() || inputs[1]->getValue()) == Wire::Value::HIGH ? Wire::Value::HIGH : Wire::Value::LOW);
	outputs[0]->markDirty();
}

// XOR
XORGate::XORGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs) : Gate(GateType::OR, inputs, outputs) {}

void XORGate::evaluate() {
	if (inputs.empty() || outputs.empty()) return;

	outputs[0]->setValue((inputs[0]->getValue() ^ inputs[1]->getValue()) == Wire::Value::HIGH ? Wire::Value::HIGH : Wire::Value::LOW);
	outputs[0]->markDirty();
}

// XNOR

XNORGate::XNORGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs) : Gate(GateType::OR, inputs, outputs) {}

void XNORGate::evaluate() {
	if (inputs.empty() || outputs.empty()) return;

	outputs[0]->setValue(!(inputs[0]->getValue() ^ inputs[1]->getValue()) == Wire::Value::HIGH ? Wire::Value::HIGH : Wire::Value::LOW);
	outputs[0]->markDirty();
}