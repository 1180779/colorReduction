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

target_link_libraries(colorReduction opengl32 glfw glad imGui stb tinyfiledialogs glm)

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

