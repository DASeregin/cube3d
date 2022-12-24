cmake_minimum_required(VERSION 3.24)
project(lab_2)

set(CMAKE_CXX_STANDARD 11)

add_executable(lab_2 main.cpp Figure.cpp Figure.h)
find_package(SFML 2.5 COMPONENTS system window graphics network audio REQUIRED)
include_directories(${SFML_INCLUDE_DIRS})
target_link_libraries(lab_2 sfml-system sfml-window sfml-graphics sfml-audio sfml-network)
