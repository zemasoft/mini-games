cmake_minimum_required(VERSION 3.12)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/clangformat-cmake/cmake)
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(ECMEnableSanitizers)

add_subdirectory(Videostop)
add_subdirectory(SlidingPuzzle)
add_subdirectory(Snake)
