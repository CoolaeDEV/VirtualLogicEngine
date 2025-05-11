#include <glad/glad.h>

#include "simulator/headers/Circuit.h"
#include "simulator/headers/Wire.h"
#include "simulator/headers/TickRunner.h"

#include "Renderer/headers/SimulationBridge.h"

#include <iostream>
#include <memory>
#include <string>
#include <queue>

#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui-node-editor/imgui_node_editor.h"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

namespace ed = ax::NodeEditor;

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

std::unique_ptr<Circuit> runBitAdder(Wire::Value A, Wire::Value B, Wire::Value CIN) {
   auto circuit = std::make_unique<Circuit>(1);

   Gate* xorGate1 = circuit->createGate(GateType::XOR);
   Gate* xorGate2 = circuit->createGate(GateType::XOR);

   Gate* andGate1 = circuit->createGate(GateType::AND);
   Gate* andGate2 = circuit->createGate(GateType::AND);

   Gate* orGate1 = circuit->createGate(GateType::OR);

   // NEED TO CREATE PROBES / SWITCHES TO CHANGE THIS PLEASSSEEEE
   Wire* wA = circuit->createWire(nullptr, 1, xorGate1);
   Wire* wB = circuit->createWire(nullptr, 2, xorGate1);
   Wire* C = circuit->createWire();

   Wire* E = circuit->createWire();

   Wire* F = circuit->createWire();
   Wire* D = circuit->createWire();

   Wire* G = circuit->createWire();
   Wire* H = circuit->createWire();

   circuit->addInputWire(wA);
   circuit->addInputWire(wB);
   circuit->addInputWire(C);

   circuit->addOutputWire(C);
   circuit->addOutputWire(H);
   wA->setValue(A);
   wB->setValue(B);
   C->setValue(CIN);

   circuit->markWireDirty(wA);
   circuit->markWireDirty(wB);
   circuit->markWireDirty(C);
   circuit->markWireDirty(D);
   circuit->markWireDirty(E);
   circuit->markWireDirty(F);
   circuit->markWireDirty(G);

   circuit->finalizeLevels();

   std::cout << "SUM output value == " << G->getValue() << std::endl;
   std::cout << "CIN output value == " << H->getValue() << std::endl;

   return circuit;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Virtual Logic Engine | v0.0.1-alpha", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// IMGUI stuff
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330 core");

	// UI editor
	ed::Config config;
	config.SettingsFile = "Simple.json";

	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD\n";
		return -1;
	}
	SimulationBridge simBridge;

	while (!glfwWindowShouldClose(window)){

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Begin("Main Window");
		ImGui::Text("FPS: %.2f (%.2gms)", io.Framerate, io.Framerate ? 1000.0f / io.Framerate : 0.0f);
		ImGui::Separator();

		simBridge.ShowNodeEditor();
		ImGui::End();
		
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.1176, 0.1294, 0.1373, 1);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);

		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}


