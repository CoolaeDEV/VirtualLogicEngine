#pragma once

#include "glad/glad.h"

#include <string>

#include <GLFW/glfw3.h>
#include "imgui/imgui.h"
#include "imgui-node-editor/imgui_node_editor.h"

#include "Renderer/headers/SimulationBridge.h"

#include "imgui/backends/imgui_impl_glfw.h"
#include "imgui/backends/imgui_impl_opengl3.h"

namespace ed = ax::NodeEditor;

static void framebuffer_size_callback(GLFWwindow* window, int width, int height);


class WindowInstance{
public:

    WindowInstance(int windowWidth, int windowHeight, std::string title, int vsync);
    ~WindowInstance() = default;

    void Init();
    void RenderLoop();
    void Cleanup();

private:
    GLFWwindow* window;
    SimulationBridge simBridge;
    
    ImGuiIO& io = ImGui::GetIO();
    int ScreenWIDTH, ScreenHEIGHT;
    std::string windowTitle;
    int isVsyncON = 0;

    bool isInitalized;
    bool isRunning;

};