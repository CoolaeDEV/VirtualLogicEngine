#include "headers/SimulationBridge.h"


const char* EnumToString(GateType value) {
	switch (value) {
	case GateType::AND: return "AND";
	case GateType::NOT: return "NOT";
	case GateType::OR: return "OR";
	case GateType::NOR: return "NOR";
	case GateType::NAND: return "NAND";
	case GateType::XOR: return "XOR";
	case GateType::XNOR: return "XNOR";
	case GateType::BUFFER: return "BUFFER";
    case GateType::Probe: return "Probe";
    case GateType::Switch: return "Switch";
	default: return "Unknown";
	}
}

SimulationBridge::~SimulationBridge() {
	delete attachedCircuit;
	attachedCircuit = nullptr;
}

void SimulationBridge::initBridge() {
	attachedCircuit = new Circuit(8);
}

void SimulationBridge::ShowNodeEditor() {
    static ed::EditorContext* context = nullptr;


    if (!context) {
        context = ed::CreateEditor();
    }

    ed::SetCurrentEditor(context);
    ed::Begin("Editor", ImVec2(0.0, 0.0f));

    // Create Nodes
    for (auto& node : nodes) {
        ed::BeginNode(node.id);
        ImGui::Text(node.name.c_str());
        SwitchGate* ourGate;

        if (!node.attachedGate) {
            ourGate = dynamic_cast<SwitchGate*>(node.attachedGate);
        }
        

        switch (node.NodeType) {

            case GateType::AND:
                ed::BeginPin(node.inputPinIDs[0], ed::PinKind::Input);
                ImGui::Text("-> In A");
                ed::EndPin();

                ImGui::SameLine();

                ed::BeginPin(node.outputPinID, ed::PinKind::Output);
                ImGui::Text("Out ->");
                ed::EndPin();

                ed::BeginPin(node.inputPinIDs[1], ed::PinKind::Input);
                ImGui::Text("-> In B");
                ed::EndPin();

                ed::EndNode();
                break;
            case GateType::NOT:
                ed::BeginPin(node.inputPinIDs[0], ed::PinKind::Input);
                ImGui::Text("-> In A");
                ed::EndPin();

                ImGui::SameLine();

                ed::BeginPin(node.outputPinID, ed::PinKind::Output);
                ImGui::Text("Out ->");
                ed::EndPin();

                ed::EndNode();
                break;
            case GateType::MacroGate:
                break;
            case GateType::BUFFER:
                ed::BeginPin(node.inputPinIDs[0], ed::PinKind::Input);
                ImGui::Text("-> In A");
                ed::EndPin();

                ImGui::SameLine();

                ed::BeginPin(node.outputPinID, ed::PinKind::Output);
                ImGui::Text("Out ->");
                ed::EndPin();

                ed::EndNode();
                break;
            case GateType::OR:
                ed::BeginPin(node.inputPinIDs[0], ed::PinKind::Input);
                ImGui::Text("-> In A");
                ed::EndPin();

                ImGui::SameLine();

                ed::BeginPin(node.outputPinID, ed::PinKind::Output);
                ImGui::Text("Out ->");
                ed::EndPin();

                ed::BeginPin(node.inputPinIDs[1], ed::PinKind::Input);
                ImGui::Text("-> In B");
                ed::EndPin();

                ed::EndNode();
                break;
            case GateType::NOR:
                ed::BeginPin(node.inputPinIDs[0], ed::PinKind::Input);
                ImGui::Text("-> In A");
                ed::EndPin();

                ImGui::SameLine();

                ed::BeginPin(node.outputPinID, ed::PinKind::Output);
                ImGui::Text("Out ->");
                ed::EndPin();

                ed::BeginPin(node.inputPinIDs[1], ed::PinKind::Input);
                ImGui::Text("-> In B");
                ed::EndPin();

                ed::EndNode();
                break;
            case GateType::NAND:
                ed::BeginPin(node.inputPinIDs[0], ed::PinKind::Input);
                ImGui::Text("-> In A");
                ed::EndPin();

                ImGui::SameLine();

                ed::BeginPin(node.outputPinID, ed::PinKind::Output);
                ImGui::Text("Out ->");
                ed::EndPin();

                ed::BeginPin(node.inputPinIDs[1], ed::PinKind::Input);
                ImGui::Text("-> In B");
                ed::EndPin();

                ed::EndNode();
                break;
            case GateType::XOR:
                ed::BeginPin(node.inputPinIDs[0], ed::PinKind::Input);
                ImGui::Text("-> In A");
                ed::EndPin();

                ImGui::SameLine();

                ed::BeginPin(node.outputPinID, ed::PinKind::Output);
                ImGui::Text("Out ->");
                ed::EndPin();

                ed::BeginPin(node.inputPinIDs[1], ed::PinKind::Input);
                ImGui::Text("-> In B");
                ed::EndPin();

                ed::EndNode();
                break;
            case GateType::XNOR:
                ed::BeginPin(node.inputPinIDs[0], ed::PinKind::Input);
                ImGui::Text("-> In A");
                ed::EndPin();

                ImGui::SameLine();

                ed::BeginPin(node.outputPinID, ed::PinKind::Output);
                ImGui::Text("Out ->");
                ed::EndPin();

                ed::BeginPin(node.inputPinIDs[1], ed::PinKind::Input);
                ImGui::Text("-> In B");
                ed::EndPin();

                ed::EndNode();
                break;
            case GateType::Switch:
                ourGate = dynamic_cast<SwitchGate*>(node.attachedGate);

                if (!ourGate) { return; }

                ed::BeginPin(node.inputPinIDs[0], ed::PinKind::Input);
                if (ImGui::Checkbox("Switch", &node.switchCheckbox)){
                    if (node.switchCheckbox) {
                        ourGate->switchedValue = Wire::Value::HIGH;
                    }
                    else {
                        ourGate->switchedValue = Wire::Value::LOW;
                    }
                }
                ed::EndPin();

                ImGui::SameLine();
                
                ed::BeginPin(node.outputPinID, ed::PinKind::Output);
                ImGui::Text("Out ->");
                ed::EndPin();

                ed::EndNode();

                break;
            case GateType::Probe:
                ProbeGate* probeGate = dynamic_cast<ProbeGate*>(node.attachedGate);
                if (!probeGate) { return; }

                ed::BeginPin(node.inputPinIDs[0], ed::PinKind::Input);
                ImGui::Text("-> In A");
                ed::EndPin();

                ImGui::SameLine();

                ed::BeginPin(node.outputPinID, ed::PinKind::Output);
                bool probedValue = (probeGate->ProbedValue == Wire::Value::HIGH);
                ImGui::Checkbox("Probe Value", &probedValue);
                ed::EndPin();

                ed::EndNode();
           break;
        }
    }

    for (auto& link : links) {
        for (int i = 0; i < link.startPinIds.size(); i++) {
            ed::Link(link.id, link.startPinIds[i], link.endPinId);
        }
    }

    //works kinda
    if (ed::BeginCreate()) {
        ed::PinId startPinId, endPinId;
        if (ed::QueryNewLink(&startPinId, &endPinId)) {
            if (startPinId && endPinId && startPinId != endPinId) {
                if (ed::AcceptNewItem()) {
                    links.push_back({ GetNextID(), static_cast<int>(startPinId.Get()), static_cast<int>(endPinId.Get())});
                    Gate* from;
                    std::vector<Gate*> to;
                    int inputIndex;

                    
                    for (Node node : nodes) {
                        // Get Origin Node
                        if (static_cast<int>(startPinId.Get()) == node.outputPinID) {
                            from = node.attachedGate;
                        }
                        // Get Ended Node
                        for (int i = 0; i < node.inputPinIDs.size(); i++) {
                            if (static_cast<int>(endPinId.Get()) == node.inputPinIDs[i]) {
                                inputIndex = i;
                                to.push_back(node.attachedGate);
                            }
                        }
                    }
                    if (from != nullptr && to.empty()) {
                        std::vector<Wire*> wires;
                        for (Gate* gate : to) {
                            wires.push_back(dynamic_cast<Wire*>(gate));
                        }
                        attachedCircuit->createWire(from, inputIndex, wires);
                    }
                }
            }
        }
    }
    ed::EndCreate();

    if (ed::BeginDelete()) {
        ed::LinkId deletedLinkId;
        while (ed::QueryDeletedLink(&deletedLinkId)) {
            if (ed::AcceptDeletedItem()) {
                links.erase(std::remove_if(links.begin(), links.end(),
                    [deletedLinkId](const Link& l) { return l.id == deletedLinkId.Get(); }),
                    links.end());
            }
        }
    }
    ed::EndDelete();

    ed::End();
    ImGui::End();

    ImGui::Begin("Node Creation Window");

    ImGui::Text("Node Creation");

    static GateType NodeType = GateType::AND;

    const char* items[] = { EnumToString(GateType::AND),
                            EnumToString(GateType::NOT), EnumToString(GateType::OR),
                            EnumToString(GateType::XOR), EnumToString(GateType::XNOR),
                            EnumToString(GateType::NAND), EnumToString(GateType::BUFFER),
                            EnumToString(GateType::NOR), EnumToString(GateType::Switch),
                            EnumToString(GateType::Probe)};

    int currentItem = static_cast<int>(NodeType);
    std::string logicGateName = EnumToString(NodeType);

    if (ImGui::Combo("Select Gate Type", &currentItem, items, IM_ARRAYSIZE(items))) {
        NodeType = static_cast<GateType>(currentItem);
    }

    if (ImGui::Button("Create Logic Gate")) {
        Node node;
        node.id = GetNextID();
        node.name = logicGateName + " " + std::to_string(node.id);
        node.NodeType = NodeType;
        switch (node.NodeType) {
            case GateType::AND:
                for (int i = 0; i < 2; i++) {
                    node.inputPinIDs.push_back(GetNextID());
                }
                node.attachedGate = attachedCircuit->createGate(NodeType);
                break;
            case GateType::NOT:
                node.inputPinIDs.push_back(GetNextID());
                node.attachedGate = attachedCircuit->createGate(NodeType);
                break;
            case GateType::MacroGate:
                break;
            case GateType::BUFFER:
                node.inputPinIDs.push_back(GetNextID());
                node.attachedGate = attachedCircuit->createGate(NodeType);
                break;
            case GateType::OR:
                for (int i = 0; i < 2; i++) {
                    node.inputPinIDs.push_back(GetNextID());
                }
                node.attachedGate = attachedCircuit->createGate(NodeType);
                break;
            case GateType::NOR:
                for (int i = 0; i < 2; i++) {
                    node.inputPinIDs.push_back(GetNextID());
                }
                node.attachedGate = attachedCircuit->createGate(NodeType);
                break;
            case GateType::NAND:
                for (int i = 0; i < 2; i++) {
                    node.inputPinIDs.push_back(GetNextID());
                }
                node.attachedGate = attachedCircuit->createGate(NodeType);
                break;
            case GateType::XOR:
                for (int i = 0; i < 2; i++) {
                    node.inputPinIDs.push_back(GetNextID());
                }
                node.attachedGate = attachedCircuit->createGate(NodeType);
                break;
            case GateType::XNOR:
                for (int i = 0; i < 2; i++) {
                    node.inputPinIDs.push_back(GetNextID());
                }
                node.attachedGate = attachedCircuit->createGate(NodeType);
                break;
            case GateType::Switch:
                node.inputPinIDs.push_back(GetNextID());
                node.attachedGate = attachedCircuit->createGate(NodeType);
                break;
            case GateType::Probe:
                node.inputPinIDs.push_back(GetNextID());
                node.attachedGate = attachedCircuit->createGate(NodeType);
                break;
        }
        node.outputPinID = GetNextID();
        nodes.push_back(node);
    }

    ImGui::End();
    ImGui::Begin("Breakpointer");
    if (ImGui::Button("Begin Breakpoint")) {
        std::cout << "Breakpoint done";
    }
    ImGui::End();
    ImGui::Begin("Logic Simulation Window");
    std::string simStatus = "Simulation Status: Stopped";
    ImGui::Text(simStatus.c_str());

    TickRunner runner([&]() {
        attachedCircuit->simulateTick();
        }, 60.0);

    if (ImGui::Button("Start Simulation")) {
        attachedCircuit->finalizeLevels();

        for (int i = 0; i < 10; i++) {
            runner.stepOnce();
        }
    }

    if (ImGui::Button("Stop Simulation")) {
        simStatus = "Simulation Status: Stopped";
        runner.stop();
    }
}

