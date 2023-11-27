cmake_minimum_required(VERSION 3.27)
project(SpaceEngine)

add_library(EngineUtils STATIC src/mesh_manager.cpp src/shader_manager.cpp src/renderer.cpp src/rigid_body.cpp src/camera.cpp src/io_processor.cpp)
target_include_directories(EngineUtils PUBLIC "${PROJECT_SOURCE_DIR}/src")
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)

add_executable(SpaceEngine src/main.cpp)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)

target_include_directories(SpaceEngine PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(SpaceEngine PUBLIC ${OPEN_GL_INCLUDE_DIRS})
target_link_libraries(SpaceEngine PUBLIC EngineUtils glfw GLEW::glew OpenGL::GL assimp compiler_flags)