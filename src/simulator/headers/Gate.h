#pragma once
#include <vector>
#include "Wire.h"

enum GateType {
	BUFFER, AND, NOT, NOR, XOR, OR, NAND, XNOR, MacroGate
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

	virtual void reset();

protected:
	const std::vector<Wire*> inputs;
	const std::vector<Wire*> outputs;

	int level = -1;

	GateType type;
};

// Gate Declaration

class BufferGate : public Gate {
public:
	BufferGate(Wire* input, std::vector<Wire*>& outputs);

	void evaluate() override;
};

class AndGate : public Gate {
public:
	AndGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs);

	void evaluate() override;
};

class ORGate : public Gate {
public:

	ORGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs);

	void evaluate() override;
};

class NotGate : public Gate {
public:
	NotGate(Wire* input, std::vector<Wire*>& outputs);

	void evaluate() override;
};

class NorGate : public Gate {
public:
	NorGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs);

	void evaluate() override;
};

class XORGate : public Gate {
public:
	XORGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs);

	void evaluate() override;
};

class NANDGate : public Gate {
public:
	NANDGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs);

	void evaluate() override;
};

class XNORGate : public Gate {
public:
	XNORGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs);

	void evaluate() override;
};