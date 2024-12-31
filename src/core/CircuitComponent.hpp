#ifndef CIRCUIT_COMPONENT_HPP
#define CIRCUIT_COMPONENT_HPP

#include "Simulation.hpp"
#include "../other/DynamicBitset.hpp"

class Event;
class Gate;
class Circuit;

class CircuitComponent {
public:
    virtual void process() = 0;
    virtual ~CircuitComponent() = default;
};

class Wire {
public:


    DynamicBitset data;
    std::vector<std::variant<Circuit*, Gate*>>* source = new std::vector<std::variant<Circuit*, Gate*>>();
    std::vector<std::variant<Circuit*, Gate*>>* destination = new std::vector<std::variant<Circuit*, Gate*>>();

    int level = 0;

    Wire(size_t wireSize) : data(wireSize) {}

    void process(size_t circuitOutputIndex);
};

class Gate : public CircuitComponent {
public:
    std::vector<DynamicBitset> inputs;
    DynamicBitset outputs;

    std::vector<Wire> inputWires;
    std::vector<Wire> outputWires;

    int level = 0;

    enum types {
        BUFFER,
        AND,
        OR,
        XOR,
        NOT,
        NAND,
        NOR,
        XNOR
    };

    types Type;

    Gate(size_t outputSize, types gateType);

    void process();
    void setInput(size_t index, const DynamicBitset& input);

    bool operator==(const Gate& other) const {
		return this == &other;
    }
    ~Gate() = default;
};

class Circuit : public CircuitComponent {
public:
    Circuit(size_t inputSize, size_t outputSize) {
        inputs.resize(inputSize);
        outputs.resize(outputSize);
    }

    std::vector<Circuit*> subCircuits;
    std::vector<Wire> wires;
    std::vector<Gate> gates;

    std::vector<DynamicBitset> inputs;
    std::vector<DynamicBitset> outputs;

    int level = 0;
	bool isInSubCircuit = false;

    void process();

    void addGate(Gate gate);
    void addWire(Wire wire);
    void addSubCircuit(Circuit* circuit);
    void connectWire(Wire* wire, std::variant<Circuit*, Gate*> source, std::variant<Circuit*, Gate*> destination);
    void levelize();

    bool operator==(Circuit* other) const {
        return  this == other;
    }
    ~Circuit() = default;

private:
    std::unordered_map<DynamicBitset, DynamicBitset> LookupTable;
};
#endif