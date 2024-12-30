#ifndef CIRCUIT_COMPONENT_HPP
#define CIRCUIT_COMPONENT_HPP

#include "Simulation.hpp"
#include "../other/DynamicBitset.hpp"

class CircuitComponent {
public:
	virtual void process() = 0;
	virtual ~CircuitComponent() = default;
};

class Wire {
public:
	DynamicBitset data;
	CircuitComponent* source = nullptr;
	CircuitComponent* destination = nullptr;
	int destinationInputIndex = 0;

	Wire(size_t wireSize) : data(wireSize) {}

	void process();
};

class Gate : public CircuitComponent {
public:
	std::vector<DynamicBitset> inputs;
	DynamicBitset outputs;

	std::vector<Wire> inputWires;
	std::vector<Wire> outputWires;

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
	void addInput(size_t inputSize);
	void setInput(size_t index, const DynamicBitset& input);
	void connectInput(Wire& wire, size_t inputIndex);
	void connectOutput(Wire& wire);
};

class Circuit : public CircuitComponent {
public:
	Circuit(size_t inputSize, size_t outputSize) : inputSize(inputSize), outputSize(outputSize) {}
	void process();
	void buildLUT();
	DynamicBitset evaluateLUT(const DynamicBitset& input);
	void addComponent(CircuitComponent* component);
	void addWire(Wire wire);

private:
	std::vector<CircuitComponent*> components;
	std::vector<Wire> wires;
	std::unordered_map<DynamicBitset, DynamicBitset> LookupTable;
	size_t inputSize;
	size_t outputSize;
};

#endif