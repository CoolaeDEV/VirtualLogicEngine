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

    // Add a new constructor matching the arguments
    Wire(Gate* From = nullptr, std::vector<int> InputIndex = std::vector<int>(), std::vector<Gate*> To = std::vector<Gate*>()) {
        // Initialize the Wire object with the provided arguments
        From = from;
        InputIndex = inputIndex;
        To = to;
    }

	uint32_t delayTicks;

	void setValue(Value newValue); // sets the current value of the wire to the ~newValue~
	Value getValue() const; // gets the current value of the wire

	void attachGate(Gate* gate);
	const std::vector<Gate*>& getAttachedGates() const;

	bool isDirty() const;
	void markDirty();
	void clearDirty();

private:
	Gate* from;
	std::vector<int> inputIndex;
	std::vector<Gate*> to;

	Value value;
	bool dirty;
	std::vector<Gate*> attachedGates;
};