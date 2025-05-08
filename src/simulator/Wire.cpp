#include "headers/Wire.h"
#include "headers/Gate.h"

Wire::Wire(Gate* from, Gate* to, uint32_t delay) : src(from), dst(to), delayTicks(delay), value(Value::LOW), dirty(false) {}

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

bool Wire::isDirty() const {
	return dirty;
}

void Wire::attachGate(Gate* gate) {
	attachedGates.push_back(gate);
}

const std::vector<Gate*>& Wire::getAttachedGates() const {
	return attachedGates;
}