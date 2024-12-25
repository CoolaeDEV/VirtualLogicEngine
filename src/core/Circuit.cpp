#include "Circuit.hpp"


Circuit::Circuit() {

}

void Circuit::AddGate(Gate gate) {
    gates.emplace_back(gate);
}

Gate Circuit::CreateGate(Gate::Type t) {
    Gate g(t);
    gates.emplace_back(g);
    return g;
}

Gate Circuit::GetGate(Gate gate) {
    return gates[gate.GateID];
}

void Circuit::ConnectCircuit(Circuit circuit, int pinID) {

}

void Circuit::Simulate() {
    //THIS DOESNT WORK AT ALL BECAUSE THERE IS NOT LEVELIZATION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    for (auto& gate : gates) {
        gate.Evaluate();
    }
}