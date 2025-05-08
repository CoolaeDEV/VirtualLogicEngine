#include "simulator/headers/Circuit.h"
#include "simulator/headers/Wire.h"
#include "simulator/headers/TickRunner.h"
#include <iostream>
#include <memory>
#include <string>

int main() {
	Circuit circuit(6);

	TickRunner runner([&]() {
		circuit.simulateTick();
	}, 60.0);

	Wire* A = circuit.createWire();
	Wire* B = circuit.createWire();
	Wire* C = circuit.createWire();

	Wire* E = circuit.createWire();

	Wire* F = circuit.createWire();
	Wire* D = circuit.createWire();
	
	Wire* G = circuit.createWire();
	Wire* H = circuit.createWire();

	circuit.addInputWire(A);
	circuit.addInputWire(B);
	circuit.addInputWire(C);

	circuit.addOutputWire(C);
	circuit.addOutputWire(H);

	std::vector<Wire*> xorInputs1 = { A, B };
	std::vector<Wire*> xorInputs2 = { E, C };

	std::vector<Wire*> andInputs1 = { E, C };
	std::vector<Wire*> andInputs2 = { A, B };

	std::vector<Wire*> orInputs = { F, D };

	Gate* xorGate1 = circuit.createGate(GateType::XOR, xorInputs1, E);
	Gate* xorGate2 = circuit.createGate(GateType::XOR, xorInputs2, G);

	Gate* andGate1 = circuit.createGate(GateType::AND, andInputs1, F);
	Gate* andGate2 = circuit.createGate(GateType::AND, andInputs2, D);

	Gate* orGate1 = circuit.createGate(GateType::OR, orInputs, H);

	A->setValue(Wire::HIGH);
	B->setValue(Wire::HIGH);
	C->setValue(Wire::HIGH);

	circuit.markWireDirty(A);
	circuit.markWireDirty(B);
	circuit.markWireDirty(C);
	circuit.markWireDirty(D);
	circuit.markWireDirty(E);
	circuit.markWireDirty(F);
	circuit.markWireDirty(G);

	circuit.finalizeLevels();

	runner.start();
	
	std::this_thread::sleep_for(std::chrono::seconds(2));
	
	runner.stop();

	std::cout << "SUM output value == " << G->getValue() << std::endl;
	std::cout << "CIN output value == " << H->getValue() << std::endl;

	std::string x;
	std::cin >> x;
	return 0;
}


