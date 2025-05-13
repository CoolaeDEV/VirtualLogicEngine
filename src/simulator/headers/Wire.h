#pragma once
#include <vector>

class Gate;

// Base Wire class (signal carrier between Gates)
class Wire {
public:
	// Possible Gate (Values / Positions)
	enum Value{
		HIGH = 1,
		LOW = 0
	};

	Wire(Gate* from = nullptr, Gate* to = nullptr, int inputIndex = 0, uint32_t delay = 1); // constructor

	uint32_t delayTicks;

	void setValue(Value newValue); // sets the current value of the wire to the ~newValue~
	Value getValue() const; // gets the current value of the wire

	void attachGate(Gate* gate);
	const std::vector<Gate*>& getAttachedGates() const;

	bool isDirty() const;
	void markDirty();
	void clearDirty();

private:
	Value value;
	bool dirty;
	std::vector<Gate*> attachedGates;
};