#include "headers/Gate.h"
#include <iostream>

Gate::Gate(const GateType GType, std::vector<Wire*>& inputs, Wire* output) : inputs(inputs), output(output), type(GType) {}

const std::vector<Wire*>& Gate::getInputs() const {
	return inputs;
}

void Gate::setLevel(int lvl) {
	level = lvl;
}

int Gate::getLevel() const{
	return level;
}

void Gate::reset() {
	output = nullptr;
}

void Gate::setDelayTicks(uint32_t ticks) {
	delayTicks = ticks;
}

uint32_t Gate::getDelayTicks() const {
	return delayTicks;
}


// Gate Implementation

// AND
AndGate::AndGate(std::vector<Wire*> inputs, Wire* output) : Gate(GateType::AND, inputs, output) {}

void AndGate::evaluate() {
	if (inputs.empty()) return;

	output->setValue((inputs[0]->getValue() && inputs[1]->getValue() == Wire::HIGH) ? Wire::HIGH : Wire::LOW);
}

// NAND
NANDGate::NANDGate(std::vector<Wire*> inputs, Wire* output) : Gate(GateType::NAND, inputs, output) {}

void NANDGate::evaluate() {
	if (inputs.empty()) return;

	output->setValue(!(inputs[0]->getValue() && inputs[1]->getValue() == Wire::HIGH) ? Wire::HIGH : Wire::LOW);
	output->markDirty();
}

// NOT
NotGate::NotGate(Wire* input, Wire* output) : Gate(GateType::NOT, std::vector<Wire*>{input}, output) {}

void NotGate::evaluate() {
	if (inputs.empty()) return;

	output->setValue(inputs[0]->getValue() == Wire::Value::HIGH ? Wire::Value::LOW : Wire::Value::HIGH);
	output->markDirty();
}

// BUFFER
BufferGate::BufferGate(Wire* input, Wire* output) : Gate(GateType::BUFFER, std::vector<Wire*>{input}, output) {}

void BufferGate::evaluate() {
	if (inputs.empty()) return;

	output->setValue(inputs[0]->getValue() == Wire::Value::HIGH ? Wire::Value::HIGH : Wire::Value::LOW);
	output->markDirty();
}

// OR
ORGate::ORGate(std::vector<Wire*> inputs, Wire* output) : Gate(GateType::OR, inputs, output){}

void ORGate::evaluate() {
	if (inputs.empty()) return;
	
	output->setValue((inputs[0]->getValue() || inputs[1]->getValue()) == Wire::Value::HIGH ? Wire::Value::HIGH : Wire::Value::LOW);
	output->markDirty();
}

// NOR
NorGate::NorGate(std::vector<Wire*> inputs, Wire* output) : Gate(GateType::NOR, inputs, output){}

void NorGate::evaluate() {
	if (inputs.empty()) return;

	output->setValue(!(inputs[0]->getValue() || inputs[1]->getValue()) == Wire::Value::HIGH ? Wire::Value::HIGH : Wire::Value::LOW);
	output->markDirty();
}

// XOR
XORGate::XORGate(std::vector<Wire*> inputs, Wire* output) : Gate(GateType::XOR, inputs, output) {}

void XORGate::evaluate() {
	if (inputs.empty()) return;

	output->setValue((inputs[0]->getValue() ^ inputs[1]->getValue()) == Wire::Value::HIGH ? Wire::Value::HIGH : Wire::Value::LOW);
	output->markDirty();
}

// XNOR

XNORGate::XNORGate(std::vector<Wire*> inputs, Wire* output) : Gate(GateType::XNOR, inputs, output) {}

void XNORGate::evaluate() {
	if (inputs.empty()) return;

	output->setValue((inputs[0]->getValue() ^ inputs[1]->getValue()) == Wire::Value::LOW ? Wire::Value::HIGH : Wire::Value::LOW);
	output->markDirty();
}

// IO Implementation

SwitchGate::SwitchGate(Wire* output) : Gate(GateType::Switch, std::vector<Wire*>{}, output){}

void SwitchGate::evaluate() {
	output->setValue(switchedValue);
	output->markDirty();
}

ProbeGate::ProbeGate(Wire* input) : Gate(GateType::Probe, std::vector<Wire*>{input}, nullptr), ProbedValue(Wire::Value::LOW) {}

void ProbeGate::evaluate() {
	inputs[0]->setValue(ProbedValue);
}