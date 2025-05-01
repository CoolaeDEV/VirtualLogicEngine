#pragma once  
#include <vector>  
#include <thread>  
#include <mutex>  
#include <atomic>  

#include "Wire.h"  
#include "Gate.h"  
#include "MacroGate.h"

class Circuit {
public:

	Wire* createWire();
	template<typename GateType, typename... Args>
	GateType* createGate(Args&&... args);

	void markWireDirty(Wire* wire);
	void simulateTick();

	const std::vector<Wire*>& getInputWires() const;
	const std::vector<Wire*>& getOutputWires() const;
	void addInputWire(Wire* wire);
	void addOutputWire(Wire* wire);

private:
	std::vector<Wire*> wires;
	std::vector<Gate*> gates;

	std::vector<Wire*> inputWires;
	std::vector<Wire*> outputWires;

	std::vector<Wire*> dirtyWires;
	std::mutex dirtyMutex;

	void processChunk(size_t start, size_t end, std::atomic<size_t>* gatesProcessed);
	size_t chunkSize;
};