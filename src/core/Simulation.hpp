#pragma once
#ifndef SIMULATION_HPP
#define SIMULATION_HPP

// Declaration of the Simulation class
// The Simulation class handles simulating the LogicEngine.
// It handles Levelization, Event Queueing, compilation of sim, and more.

#include <vector>
#include <bitset>
#include <unordered_map>
#include <memory>
#include <algorithm>
#include <cstdint>
#include <queue>
#include <functional>
#include <string>
#include "../other/DynamicBitset.hpp"
#include "CircuitComponent.hpp"

class Circuit;
enum class SimulationMode;

template <size_t INPUT_SIZE, size_t OUTPUT_SIZE>
class LookupTable {
private:
    std::unordered_map<std::bitset<INPUT_SIZE>, std::bitset<OUTPUT_SIZE>> table;
public:
    void set(const std::bitset<INPUT_SIZE>& input, const std::bitset<OUTPUT_SIZE>& output) {
        table[input] = output;
    }

    std::bitset<OUTPUT_SIZE> get(const std::bitset<INPUT_SIZE>& input) const {
        auto it = table.find(input);
        if (it != table.end()) {
            return it->second;
        }
        return std::bitset<OUTPUT_SIZE>{};
    }

    void clear() {
        table.clear();
    }
};

class Simulation {
public:
    Simulation(SimulationMode initMode);
    ~Simulation() = default;

    SimulationMode mode;

    std::vector<Circuit*> circuits;

    void setMode(SimulationMode newMode) {
        mode = newMode;
    }
    void addCircuit(Circuit* newCircuit) {
        circuits.push_back(newCircuit);
    }

    void tick();
private:
    void simulateRealtime();
    void buildCompiledLUT();
    void evaluteCompiledLUT();
};

enum class SimulationMode {
    Realtime,
    Compiled
};

#endif