cmake_minimum_required(VERSION 3.30)
project(colorReduction)

set(CMAKE_CXX_STANDARD 17)


#--------------------------------------------------------------------
# source
#--------------------------------------------------------------------
file(GLOB_RECURSE SOURCE
        src/*.hpp src/*.h
        src/*.cpp src/*.c)

add_executable(colorReduction ${SOURCE})

add_subdirectory(deps)

find_package(OpenGL REQUIRED COMPONENTS OpenGL)

target_link_libraries(colorReduction OpenGL glfw glad imGui stb tinyfiledialogs glm)

add_custom_command(
        TARGET colorReduction POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/src/windows/sh.vert
        ${CMAKE_CURRENT_BINARY_DIR}/sh.vert)

add_custom_command(
        TARGET colorReduction POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/src/windows/sh.frag
        ${CMAKE_CURRENT_BINARY_DIR}/sh.frag)

add_custom_command(
        TARGET colorReduction POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/src/windows/GL_RGBA8.comp
        ${CMAKE_CURRENT_BINARY_DIR}/GL_RGBA8.comp)
