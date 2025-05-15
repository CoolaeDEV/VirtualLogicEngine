#pragma once  

#include <vector>  
#include <thread>  
#include <mutex>  
#include <atomic>  

#include "Wire.h"  
#include "Gate.h"  
#include "MacroGate.h"
#include "ThreadPool.h"


static constexpr size_t MAX_DELAY_TICKS = 64;


struct Event {
	Gate* gate;
	bool lastOutput;

	Event(Gate* g, bool prevOut) : gate(g), lastOutput(prevOut) {}
};

class Circuit {
public:

	Circuit(size_t threads) : threadCount(threads), threadPool(threads), chunkSize(128), tickQueue(MAX_DELAY_TICKS){};

	Wire* createWire(Gate* from = nullptr, std::vector<int> inputIndex = std::vector<int>(), std::vector<Gate*> to = std::vector<Gate*>());

	Gate* createGate(GateType Gtype);

	void markWireDirty(Wire* wire);
	void simulateTick();

	void scheduleGate(Gate* gate, uint32_t delay);

	const std::vector<Wire*>& getInputWires() const;
	const std::vector<Wire*>& getOutputWires() const;
	void addInputWire(Wire* wire);
	void addOutputWire(Wire* wire);
	void reset();

	void finalizeLevels();

	std::vector<Wire*> wires;
	std::vector<Gate*> gates;

private:
	std::vector<Wire*> inputWires;
	std::vector<Wire*> outputWires;

	std::vector<Wire*> dirtyWires;
	std::mutex dirtyMutex;

	std::vector<std::vector<Gate*>> levelizedGates;
	bool levelsFinalized = false;

	std::vector<std::vector<Event>> tickQueue;
	size_t currentTickIndex = 0;
	size_t tickMod = MAX_DELAY_TICKS;

	size_t chunkSize;
	size_t threadCount;

	ThreadPool threadPool;
};