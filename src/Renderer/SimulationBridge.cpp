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
        }
    }

    for (auto& link : links) {
        for (int i = 0; i < link.startPinIds.size(); i++) {
            ed::Link(link.id, link.startPinIds[i], link.endPinId);
        }
    }

    // NEEDS TO BE FIXED SO THAT I CAN HAVE THE NODES TAKE INPUT FROM THE LINKS AS WIRES
    if (ed::BeginCreate()) {
        ed::PinId startPinID, endPinID;
        if (ed::QueryNewLink(&startPinID, &endPinID)) {
            if (ed::QueryNewLink(&startPinID, &endPinID)) {  
               if (startPinID && endPinID && startPinID != endPinID) {  
                   if (ed::AcceptNewItem()) {  
                       auto startPinIndex = std::find_if(links.begin(), links.end(),  
                           [startPinID](const Link& l) { return l.id == startPinID.Get(); });  
                       if (startPinIndex != links.end()) {
                           links.push_back({ GetNextID(), static_cast<int>(startPinID.Get()), static_cast<int>(endPinID.Get()) });
                           attachedCircuit->createWire(nullptr, startPinID.Get(), nullptr);
                       }
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
                            EnumToString(GateType::NOR) };

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
                attachedCircuit->createGate(NodeType);
                break;
            case GateType::NOT:
                node.inputPinIDs.push_back(GetNextID());
                attachedCircuit->createGate(NodeType);
                break;
            case GateType::MacroGate:
                break;
            case GateType::BUFFER:
                node.inputPinIDs.push_back(GetNextID());
                attachedCircuit->createGate(NodeType);
                break;
            case GateType::OR:
                for (int i = 0; i < 2; i++) {
                    node.inputPinIDs.push_back(GetNextID());
                }
                attachedCircuit->createGate(NodeType);
                break;
            case GateType::NOR:
                for (int i = 0; i < 2; i++) {
                    node.inputPinIDs.push_back(GetNextID());
                }
                attachedCircuit->createGate(NodeType);
                break;
            case GateType::NAND:
                for (int i = 0; i < 2; i++) {
                    node.inputPinIDs.push_back(GetNextID());
                }
                attachedCircuit->createGate(NodeType);
                break;
            case GateType::XOR:
                for (int i = 0; i < 2; i++) {
                    node.inputPinIDs.push_back(GetNextID());
                }
                attachedCircuit->createGate(NodeType);
                break;
            case GateType::XNOR:
                for (int i = 0; i < 2; i++) {
                    node.inputPinIDs.push_back(GetNextID());
                }
                attachedCircuit->createGate(NodeType);
                break;
        }
        node.outputPinID = GetNextID();
        nodes.push_back(node);
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

        runner.start();
    }

    if (ImGui::Button("Stop Simulation")) {
        simStatus = "Simulation Status: Stopped";
        runner.stop();
    }
}

