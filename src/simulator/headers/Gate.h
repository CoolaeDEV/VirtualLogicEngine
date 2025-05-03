#pragma once
#include <vector>
#include "Wire.h"

enum GateType {
	AND, NOT, MacroGate
};
class Gate {
public:
	Gate(const GateType type, std::vector<Wire*>& inputs, std::vector<Wire*>& outputs);

	virtual ~Gate() = default;

	virtual void evaluate() = 0;

	const std::vector<Wire*>& getInputs() const;
	const std::vector<Wire*>& getOutputs() const;

	void setLevel(int lvl);
	int getLevel() const;

protected:
	const std::vector<Wire*> inputs;
	const std::vector<Wire*> outputs;

	int level = -1;

	GateType type;
};

// Gate Declaration

class AndGate : public Gate {
public:
	AndGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs);

	void evaluate() override;
};

class NotGate : public Gate {
public:
	NotGate(Wire* input, std::vector<Wire*>& outputs);

	void evaluate() override;
};