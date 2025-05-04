#include "simulator/headers/Circuit.h"
#include "simulator/headers/Wire.h"
#include "simulator/headers/TickRunner.h"
#include <iostream>
#include <memory>
#include <string>

int main() {
	Circuit circuit(8);

	TickRunner runner([&]() {
		circuit.simulateTick();
		}, 10.0);

	Wire* A = circuit.createWire();
	Wire* B = circuit.createWire();
	Wire* C = circuit.createWire();
	Wire* OUT = circuit.createWire();

	circuit.addInputWire(A);
	circuit.addOutputWire(OUT);

	std::vector<Wire*> ANDinputs = { A };
	std::vector<Wire*> ANDoutput = { B };

	std::vector<Wire*> NOTinputs = { B };
	std::vector<Wire*> NOToutputs = { C };

	std::vector<Wire*> AND2inputs = { A, C };
	std::vector<Wire*> AND2output = { OUT };

	Gate* andGate = circuit.createGate(GateType::BUFFER, ANDinputs, ANDoutput);
	Gate* notGate = circuit.createGate(GateType::NOT, NOTinputs, NOToutputs);
	Gate* andGate2 = circuit.createGate(GateType::AND, AND2inputs, AND2output);

	A->setValue(Wire::LOW);

	circuit.markWireDirty(A);
	circuit.markWireDirty(B);
	circuit.markWireDirty(C);

	circuit.finalizeLevels();

	runner.start();

	std::this_thread::sleep_for(std::chrono::seconds(2));

	runner.stop();

	std::cout << "circuit output value == " << OUT->getValue();

	std::string x;
	std::cin >> x;
	return 0;
}


