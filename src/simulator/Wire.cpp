#include "headers/Wire.h"
#include "headers/Gate.h"

Wire::Wire() : value(Value::LOW), dirty(false) {}

void Wire::setValue(Value newValue) {
	if (value != newValue) {
		value = newValue;
		markDirty();
	}
}

Wire::Value Wire::getValue() const {
	return value;
}

void Wire::markDirty() {
	dirty = true;
}

void Wire::clearDirty() {
	dirty = false;
}

void Wire::attachGate(Gate* gate) {
	attachedGates.push_back(gate);
}

const std::vector<Gate*>& Wire::getAttachedGates() const {
	return attachedGates;
}