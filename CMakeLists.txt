cmake_minimum_required(VERSION 3.18)
project(mace)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake-modules/android/")

add_subdirectory(engine)
add_subdirectory(game)