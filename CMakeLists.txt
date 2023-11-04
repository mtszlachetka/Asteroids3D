cmake_minimum_required(VERSION 3.27)
project(3DApp)

add_library(EngineUtils STATIC src/ModelLoader.cpp src/ShaderLoader.cpp src/Body.hpp src/System.cpp src/Camera.cpp src/IOProcessor.cpp src/Renderer.cpp)
target_include_directories(EngineUtils PUBLIC "${PROJECT_SOURCE_DIR}/src")
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)

add_executable(App src/main.cpp)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

target_include_directories(App PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(App PUBLIC ${OPEN_GL_INCLUDE_DIRS})
target_link_libraries(App PUBLIC EngineUtils glfw GLEW::glew OpenGL::GL assimp compiler_flags)