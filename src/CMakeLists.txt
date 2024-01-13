add_subdirectory(subengines)
add_subdirectory(gameplay)
add_library(EngineUtils STATIC mesh.cpp shader.cpp texture.cpp read_file.cpp camera.cpp hud.cpp)
target_include_directories(EngineUtils PUBLIC "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/lib/SOIL" )