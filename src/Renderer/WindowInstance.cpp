
#include "headers/WindowInstance.h"


static void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    glViewport(0, 0, width, height);
}
WindowInstance::WindowInstance(int windowWidth, int windowHeight, std::string title, int vsync){
    ScreenWIDTH = windowWidth;
    ScreenHEIGHT = windowHeight;
    windowTitle = title;
    isVsyncON = vsync;
}


void WindowInstance::Init(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(ScreenWIDTH, ScreenHEIGHT, windowTitle.c_str(), NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(isVsyncON);

    // IMGUI

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    ed::Config config;
    config.SettingsFile = "Simple.json";

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        std::cout << "[ WINDOW_INSTANCE ] Failed to start GLAD\n";
        isInitalized = false;
    }
    SimulationBridge simBridge;
    isInitalized = true;
}

void WindowInstance::RenderLoop(){
    if(!isInitalized){
        std::cout << "[ WINDOW_INSTANCE ] The window instance has not initalized.";
    }else{
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
    }
}

void WindowInstance::Cleanup(){
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
}