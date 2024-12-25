#include "Circuit.hpp"


// Initalization of the Circuit class and its methods
// The Circuit class holds gates, wires, pins, and other circuits, to be evaluated.


// Circuit constructer
Circuit::Circuit() {

}


// Adds an existing gate to the circuit (adds it to the gate vector array)
void Circuit::AddGate(Gate gate) {
    gates.emplace_back(gate);
}

// Adds a new gate to the circuit (adds it to the gate vector array)
Gate Circuit::CreateGate(Gate::Type t) {
    Gate g(t);
    gates.emplace_back(g);
    return g;
}

// Returns a gate from the gate vector array
Gate Circuit::GetGate(Gate gate) {
    return gates[gate.GateID];
}

// Connects a circuit to another circuit via the circuit Pin(s)
void Circuit::ConnectCircuit(Circuit circuit, int pinID) {

}

//Simulate the circuit (entire simulation for that circuit itself)
void Circuit::Simulate() {
    //THIS DOESNT WORK AT ALL BECAUSE THERE IS NOT LEVELIZATION!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    
    for (auto& gate : gates) {
        gate.Evaluate();
    }
}