
// Declaration of the Circuit class
// The Circuit class holds gates, wires, pins, and other circuits, to be evaluated.
#include <vector>

#include "Device.hpp"
#include <unordered_map>
#include <memory> 
#include <algorithm>

class Circuit {
public:
    Circuit();
    ~Circuit() = default;

    void AddGate(Gate gate);
    Gate CreateGate(Gate::Type t);
    Gate GetGate(Gate gate);
    void ConnectCircuit(Circuit circuit, int pinID);
    void Simulate();

private:
    std::vector<Gate> gates;
    std::vector<Wire> wires;
    std::vector<Circuit> circuits;
    std::vector<Pin> circuitPins;
    int level;


};