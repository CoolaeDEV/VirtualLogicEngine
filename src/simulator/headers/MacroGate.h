#pragma once
#include "Gate.h"
#include "Circuit.h"

class Circuit;

class MacroGate : public Gate {
public:
	MacroGate(std::vector<Wire*>& inputs, Wire* output, Circuit* internalCircuit);

	void evaluate() override;

private:
	Circuit* circuit;
};