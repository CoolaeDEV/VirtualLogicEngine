#pragma once  
#include <vector>  
#include <thread>  
#include <mutex>  
#include <atomic>  

#include "Wire.h"  
#include "Gate.h"  
#include "MacroGate.h"
#include "ThreadPool.h"

class Circuit {
public:

	Circuit(size_t threads) : threadCount(threads), threadPool(threads), chunkSize(128) {};

	Wire* createWire();

	Gate* createGate(GateType Gtype, std::vector<Wire*>& inputs, std::vector<Wire*>& outputs);

	void markWireDirty(Wire* wire);
	void simulateTick();

	const std::vector<Wire*>& getInputWires() const;
	const std::vector<Wire*>& getOutputWires() const;
	void addInputWire(Wire* wire);
	void addOutputWire(Wire* wire);
	void reset();

	void finalizeLevels();

private:
	std::vector<Wire*> wires;
	std::vector<Gate*> gates;

	std::vector<Wire*> inputWires;
	std::vector<Wire*> outputWires;

	std::vector<Wire*> dirtyWires;
	std::mutex dirtyMutex;

	std::vector<std::vector<Gate*>> levelizedGates;
	bool levelsFinalized = false;

	void processChunk(size_t start, size_t end, std::atomic<size_t>* gatesProcessed);
	size_t chunkSize;
	size_t threadCount;

	ThreadPool threadPool;
};