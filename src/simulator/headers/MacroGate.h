#pragma once
#include "Gate.h"
#include "simulator/headers/Circuit.h"

class Circuit;

class MacroGate : public Gate {
public:
	MacroGate(std::vector<Wire*>& inputs, std::vector<Wire*>& outputs, Circuit* internalCircuit);

	void evaluate() override;

private:
	Circuit* circuit;
};