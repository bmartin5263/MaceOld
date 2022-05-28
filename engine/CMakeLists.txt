cmake_minimum_required(VERSION 3.18)
project(engine)

set(CMAKE_CXX_STANDARD 14)

IF(ANDROID)
    include(AndroidNdkModules)
    android_ndk_import_module_native_app_glue()
    message(STATUS "Mace detected Android build")
    add_library(engine src/Engine.cpp ndk/src/Log.cpp)
    target_link_libraries(engine PUBLIC
            native_app_glue
            GLESv3
            EGL
            log
            android
            atomic
    )
    set_target_properties(
            engine
            PROPERTIES
            CXX_STANDARD 14
            CXX_STANDARD_REQUIRED YES
            CXX_EXTENSIONS NO
            INTERFACE_INCLUDE_DIRECTORIES $<TARGET_PROPERTY:engine,INCLUDE_DIRECTORIES>
    )
ELSEIF(IOS)
    message(STATUS "Mace detected IOS build")
    add_library(engine src/Engine.cpp ios/src/Log.cpp)
ELSE()
    message(STATUS "Mace detected idk build")
    add_library(engine src/Engine.cpp)
ENDIF()

target_include_directories(engine PUBLIC include)
target_include_directories(engine PRIVATE bridge)