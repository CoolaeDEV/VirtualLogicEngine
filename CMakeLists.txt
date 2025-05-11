cmake_minimum_required(VERSION 3.16)
project(VirtualLogicEngine)

set(CMAKE_CXX_STANDARD 17)

set(LIBRARY_DIR "${CMAKE_SOURCE_DIR}/Libs")

set(IMGUI_DIR "${CMAKE_SOURCE_DIR}/IncludeDir/imgui")

set(IMGUI_NODEEDITOR_DIR "${CMAKE_SOURCE_DIR}/IncludeDir/imgui-node-editor")

set(IMGUI_NODEEDITORSOURCES
	${IMGUI_NODEEDITOR_DIR}/crude_json.cpp
	${IMGUI_NODEEDITOR_DIR}/imgui_canvas.cpp
	${IMGUI_NODEEDITOR_DIR}/imgui_node_editor.cpp
	${IMGUI_NODEEDITOR_DIR}/imgui_node_editor_api.cpp
)

set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
	${IMGUI_DIR}/imgui_demo.cpp
	${IMGUI_DIR}/imgui_draw.cpp
	${IMGUI_DIR}/imgui_tables.cpp
	${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_opengl3.cpp
)

include_directories(src, IncludeDir)

file(GLOB SOURCES src/*.cpp)
file(GLOB LIBRARIES "${LIBRARY_DIR}/*.lib" "${LIBRARY_DIR}/*.a")

add_executable(VirtualLogicEngine
			src/main.cpp
			src/simulator/Circuit.cpp
			src/simulator/Gate.cpp
			src/simulator/MacroGate.cpp
			src/simulator/Wire.cpp 
			"src/simulator/ThreadPool.cpp" 
			"src/simulator/headers/ThreadPool.h" 
			"src/simulator/TickRunner.cpp" 
			"src/simulator/headers/TickRunner.h"  
			"src/simulator/examples/adder_test.cpp"
			"src/glad.c"
			${IMGUI_SOURCES}
			${IMGUI_NODEEDITORSOURCES} "src/Renderer/SimulationBridge.cpp" "src/Renderer/headers/SimulationBridge.h")

target_include_directories(VirtualLogicEngine PRIVATE IncludeDir)
target_link_libraries(VirtualLogicEngine PRIVATE ${LIBRARIES})