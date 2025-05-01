#pragma once
#include <vector>
#include "Wire.h"

class Gate {
public:
	Gate(const std::vector<Wire*>& inputs, const std::vector<Wire*>& outputs);

	virtual ~Gate() = default;

	virtual void evaluate() = 0;

	const std::vector<Wire*>& getInputs() const;
	const std::vector<Wire*>& getOutputs() const;

protected:
	std::vector<Wire*> inputs;
	std::vector<Wire*> outputs;
};

// Gate Declaration

class AndGate : public Gate {
public:
	AndGate(const std::vector<Wire*>& inputs, Wire* output);

	void evaluate() override;
};

class NotGate : public Gate {
public:
	NotGate(Wire* input, Wire* output);

	void evaluate() override;
};