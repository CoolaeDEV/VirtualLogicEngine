#pragma once
#include <string>
#include <vector>

#include "imgui/imgui.h"
#include "imgui-node-editor/imgui_node_editor.h"
#include "C:\Users\johnsonmk0713\Documents\GitHub\VirtualLogicEngine\src\simulator\headers\Circuit.h"
#include "C:\Users\johnsonmk0713\Documents\GitHub\VirtualLogicEngine\src\simulator\headers\Gate.h"
#include "C:\Users\johnsonmk0713\Documents\GitHub\VirtualLogicEngine\src\simulator\headers\TickRunner.h"

namespace ed = ax::NodeEditor;

struct Node {
	int id;
	std::string name;
	std::vector<int> inputPinIDs;
	int outputPinID;

	std::vector<Wire*> inputs;
	Wire* output;

	bool switchCheckbox;

	GateType NodeType;

	Gate* attachedGate;
};

struct Link {
	int id;
	std::vector<int> startPinIds;
	int endPinId;
	Link(int id, int startPinId, int endPinId) : id(id), endPinId(endPinId) {
		startPinIds.push_back(startPinId);
	}
};

class SimulationBridge {
public:
	SimulationBridge() : attachedCircuit(new Circuit(4)) {}
	~SimulationBridge();

	int GetNextID() { return nextId++; }
	void ShowNodeEditor();
	void initBridge();
private:
	Circuit* attachedCircuit;
	int nextId = 1;
	std::vector<Node> nodes;
	std::vector<Link> links;
};