add_subdirectory(SOIL)
add_library(EngineUtils STATIC mesh_manager.cpp shader_manager.cpp renderer.cpp rigid_body.cpp camera.cpp io_processor.cpp texture_manager.cpp)
target_include_directories(EngineUtils PRIVATE SOIL)
target_include_directories(EngineUtils PUBLIC "${PROJECT_SOURCE_DIR}/include")