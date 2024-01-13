add_library(Gameplay STATIC player.cpp asteroid.cpp missile.cpp station.cpp)
target_include_directories(Gameplay PUBLIC "${PROJECT_SOURCE_DIR}/include")