cmake_minimum_required(VERSION 3.16)
project(VirtualLogicSimulator)

set(CMAKE_CXX_STANDARD 17)


include_directories(src)

file(GLOB SOURCES src/*.cpp)


add_executable(VirtualLogicSimulator
	src/main.cpp
	src/simulator/Circuit.cpp
	src/simulator/Gate.cpp
	src/simulator/MacroGate.cpp
	src/simulator/Wire.cpp
	src/opencl/OpenCLManager.cpp
	src/bnn/macro_recognizer.cpp)