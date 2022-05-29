include(AndroidNdkModules)
android_ndk_import_module_native_app_glue()

file (GLOB_RECURSE Mace-Sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file (GLOB_RECURSE Mace-Bridge CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/bridge/*.h")
file (GLOB_RECURSE Mace-Ndk-Sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ndk/src/*.cpp")
file (GLOB_RECURSE Mace-Ndk-Headers CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/ndk/src/*.h")
#file (GLOB_RECURSE Foo_HEADERS CONFIGURE_DEPENDS "src/*.h")
#
#set (Foo_INCLUDE_DIRS "")
#foreach (_headerFile ${Foo_HEADERS})
#    get_filename_component(_dir ${_headerFile} PATH)
#    list (APPEND Foo_INCLUDE_DIRS ${_dir})
#endforeach()
#list (REMOVE_DUPLICATES Foo_INCLUDE_DIRS)

add_library(engine
        ${Mace-Sources}
        ${Mace-Ndk-Sources}
        )

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
target_include_directories(engine PUBLIC ndk/src)
