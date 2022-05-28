cmake_minimum_required(VERSION 3.21)
project(game)

set(CMAKE_CXX_STANDARD 17)

add_library(game src/Game.cpp)
target_include_directories(game PUBLIC include)
target_link_libraries(game PUBLIC engine)