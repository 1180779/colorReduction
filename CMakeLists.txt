cmake_minimum_required(VERSION 3.30)
project(colorReduction)

set(CMAKE_CXX_STANDARD 17)

#--------------------------------------------------------------------
# dependencies
#--------------------------------------------------------------------
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(imGui)

include_directories(glad/include)   # glad headers
include_directories(glfw/include)   # glfw headers
include_directories(imGui/include)  # imGui headers

#--------------------------------------------------------------------
# source
#--------------------------------------------------------------------
file(GLOB_RECURSE SOURCE
        source/*.hpp source/*.h
        source/*.cpp source/*.c)

add_executable(colorReduction ${SOURCE})

target_link_libraries(colorReduction opengl32 glfw glad imGui)
