cmake_minimum_required(VERSION 3.18)
project(engine)

set(CMAKE_CXX_STANDARD 14)

IF(ANDROID)
    message(STATUS "Mace detected Android build")
    include(ndk/MaceNdk.cmake)
ELSEIF(IOS)
    message(STATUS "Mace detected IOS build")
    add_library(engine src/Engine.cpp ios/src/Log.cpp)
ELSE()
    message(STATUS "Mace detected idk build")
    add_library(engine src/Engine.cpp)
ENDIF()

target_include_directories(engine PUBLIC include)
target_include_directories(engine PUBLIC bridge)