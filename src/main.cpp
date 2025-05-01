#include "simulator/headers/Circuit.h"
#include "simulator/headers/Wire.h"
#include <iostream>
#include <memory>
#include <string>

int main() {
	Circuit circuit;

	Wire* A = circuit.createWire();
	Wire* B = circuit.createWire();
	Wire* OUT = circuit.createWire();

	circuit.addInputWire(A);
	circuit.addInputWire(B);
	circuit.addOutputWire(OUT);

	std::vector<Wire*> inputs = { A, B };
	std::vector<Wire*> outputs = { OUT };
	circuit.createGate<AndGate>(inputs, outputs);

	A->setValue(Wire::HIGH);
	B->setValue(Wire::HIGH);

	circuit.markWireDirty(A);
	circuit.markWireDirty(B);

	circuit.simulateTick();

	std::cout << "A: " << A->getValue() << ", B: " << B->getValue() << "=> OUT: " << OUT->getValue() << std::endl;

	std::string x;
	std::cin >> x;

	return 0;
}