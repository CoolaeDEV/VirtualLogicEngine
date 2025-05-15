
#include "simulator/headers/Circuit.h"
#include "simulator/headers/Wire.h"
#include "simulator/headers/TickRunner.h"

#include "Renderer/headers/SimulationBridge.h"

#include <iostream>
#include <memory>
#include <string>
#include <queue>

std::unique_ptr<Circuit> runBitAdder(Wire::Value A, Wire::Value B, Wire::Value CIN) {
   auto circuit = std::make_unique<Circuit>(1);

   TickRunner runner([&]{
	  circuit->simulateTick();
   }, 60.0);

   Gate* switch1 = circuit->createGate(GateType::Switch);
   Gate* switch2 = circuit->createGate(GateType::Switch);
   Gate* switch3 = circuit->createGate(GateType::Switch);

   dynamic_cast<SwitchGate*>(switch1)->switchedValue = A;
   dynamic_cast<SwitchGate*>(switch2)->switchedValue = B;
   dynamic_cast<SwitchGate*>(switch3)->switchedValue = CIN;

   Gate* xor1 = circuit->createGate(GateType::XOR);
   Gate* xor2 = circuit->createGate(GateType::XOR);

   Gate* and1 = circuit->createGate(GateType::AND);
   Gate* and2 = circuit->createGate(GateType::AND);
   Gate* or1 = circuit->createGate(GateType::OR);

   Wire* A_wire = circuit->createWire(switch1, { 0, 0 }, { xor1, and2 });
   Wire* B_wire = circuit->createWire(switch2, { 1, 1 }, { xor1, and2 });
   Wire* CIN_wire = circuit->createWire(switch3, { 1, 1 }, { xor2, and1 });
   
   circuit->finalizeLevels();

   circuit->markWireDirty(A_wire);
   circuit->markWireDirty(B_wire);
   circuit->markWireDirty(CIN_wire);

   
   runner.start();

	std::this_thread::sleep_for(std::chrono::seconds(3));
	runner.stop();

   //std::cout << "SUM output value == " << G->getValue() << std::endl;
   //std::cout << "CIN output value == " << H->getValue() << std::endl;

   return circuit;
}

int main() {
   auto c = runBitAdder(Wire::Value::HIGH, Wire::Value::LOW, Wire::Value::HIGH);
   return 0;
}


