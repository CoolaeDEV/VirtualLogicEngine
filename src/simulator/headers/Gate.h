#pragma once
#include <vector>
#include "Wire.h"

enum GateType {
	BUFFER, AND, NOT, NOR, XOR, OR, NAND, XNOR, MacroGate
};
class Gate {
public:
	Gate(const GateType type, std::vector<Wire*>& inputs, Wire* output);

	virtual ~Gate() = default;

	virtual void evaluate() = 0;
	virtual void reset();

	const std::vector<Wire*>& getInputs() const;
	const Wire* getOutput() const { return output; }

	const std::vector<Wire*> inputs;
	Wire* output;

	void setLevel(int lvl);
	int getLevel() const;

	void setDelayTicks(uint32_t ticks);
	uint32_t getDelayTicks() const;



protected:
	int level = -1;

	GateType type;

	uint32_t delayTicks = 1;
};

// Gate Declaration

class BufferGate : public Gate {
public:
	BufferGate(Wire* input, Wire* output);

	void evaluate() override;
};

class AndGate : public Gate {
public:
	AndGate(std::vector<Wire*> inputs, Wire* output);

	void evaluate() override;
};

class ORGate : public Gate {
public:

	ORGate(std::vector<Wire*> inputs, Wire* output);

	void evaluate() override;
};

class NotGate : public Gate {
public:
	NotGate(Wire* input, Wire* output);

	void evaluate() override;
};

class NorGate : public Gate {
public:
	NorGate(std::vector<Wire*> inputs, Wire* output);

	void evaluate() override;
};

class XORGate : public Gate {
public:
	XORGate(std::vector<Wire*> inputs, Wire* output);

	void evaluate() override;
};

class NANDGate : public Gate {
public:
	NANDGate(std::vector<Wire*> inputs, Wire* output);

	void evaluate() override;
};

class XNORGate : public Gate {
public:
	XNORGate(std::vector<Wire*> inputs, Wire* output);

	void evaluate() override;
};