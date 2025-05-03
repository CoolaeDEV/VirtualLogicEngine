#include "simulator/headers/Circuit.h"
#include "simulator/headers/Wire.h"
#include <iostream>
#include <memory>
#include <string>

int main() {
	Circuit circuit(8);

	Wire* A = circuit.createWire();
	Wire* B = circuit.createWire();
	Wire* C = circuit.createWire();
	Wire* OUT = circuit.createWire();

	circuit.addInputWire(A);
	circuit.addInputWire(B);
	circuit.addOutputWire(OUT);

	std::vector<Wire*> ANDinputs = { A, B };
	std::vector<Wire*> ANDoutput = { C };
	
	std::vector<Wire*> NOTInput = { C };
	std::vector<Wire*> NOTOutput = { OUT };
	Gate* andGate = circuit.createGate(GateType::AND, ANDinputs, ANDoutput);
	Gate* notGate = circuit.createGate(GateType::NOT, NOTInput, NOTOutput);

	A->setValue(Wire::LOW);
	B->setValue(Wire::HIGH);

	circuit.markWireDirty(A);
	circuit.markWireDirty(B);
	circuit.markWireDirty(C);


	circuit.finalizeLevels();

	for (int i = 0; i < 10; i++) {
		std::cout << "[ SIM TICK " << i << " ]\n";
		std::cout << "AND Input[0] = " << A->getValue() << " AND Input[1] = " << B->getValue() << " AND Output = " << C->getValue() << "\n";
		std::cout << "NOT Input = " << C->getValue() << " NOT Output = " << OUT->getValue() << "\n\n";

		if (i == 5) {
			A->setValue(Wire::HIGH);
			circuit.markWireDirty(A);
			circuit.markWireDirty(B);
			circuit.markWireDirty(C);
		}
		circuit.simulateTick();
	}
	//std::cout << "A: " << A->getValue() << ", B: " << B->getValue() << " => OUT: " << OUT->getValue() << std::endl;

	std::string x;
	std::cin >> x;
	return 0;
}