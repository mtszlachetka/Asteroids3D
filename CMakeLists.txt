cmake_minimum_required(VERSION 3.27)
project(Asteroids3D)

add_subdirectory(src)
add_subdirectory(lib)
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE cxx_std_20)

add_executable(${CMAKE_PROJECT_NAME} src/main.cpp)

find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(OpenGL REQUIRED)
find_package(assimp REQUIRED)
find_package(Freetype REQUIRED)

target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${GLEW_INCLUDE_DIRS})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${OPEN_GL_INCLUDE_DIRS})
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${FREETYPE_INCLUDE_DIRS})
target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC EngineUtils Subengines Gameplay SOIL glfw GLEW::glew OpenGL::GL assimp compiler_flags ${FREETYPE_LIBRARIES})