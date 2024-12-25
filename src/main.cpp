#include <iostream>
#include "core/Circuit.hpp"

int main() {
	Circuit c;
	Gate g(Gate::Type::AND);
	g.inputs[0].value = { 0b1 };
	g.inputs[1].value = { 0b1 };
	c.AddGate(g);
	c.Simulate();
	
	return 0;
}