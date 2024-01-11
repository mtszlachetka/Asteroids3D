add_library(EngineUtils STATIC mesh.cpp shader.cpp texture.cpp read_file.cpp)
target_include_directories(EngineUtils PUBLIC "${PROJECT_SOURCE_DIR}/include" "${PROJECT_SOURCE_DIR}/lib/SOIL" )