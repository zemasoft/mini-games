cmake_minimum_required(VERSION 3.12)

project(mini-games)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

add_subdirectory(Videostop)
add_subdirectory(SlidingPuzzle)
add_subdirectory(Snake)
