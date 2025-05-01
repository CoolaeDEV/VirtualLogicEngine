#pragma once
#include "Gate.h"
#include "simulator/headers/Circuit.h"

class MacroGate : public Gate {
public:
	MacroGate(const std::vector<Wire*>& inputs, const std::vector<Wire*>& outputs, Circuit* internalCircuit);

	void evaluate() override;

private:
	Circuit* circuit;
};