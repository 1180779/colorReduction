#--------------------------------------------------------------------
# dependencies
#--------------------------------------------------------------------
add_subdirectory(glad)
add_subdirectory(glfw)
add_subdirectory(imGui)
add_subdirectory(stb)
add_subdirectory(tinyfiledialogs)
add_subdirectory(glm)

target_include_directories(colorReduction PUBLIC glad/include)               # glad headers
target_include_directories(colorReduction PUBLIC glfw/include)               # glfw headers
target_include_directories(colorReduction PUBLIC imGui/include)              # imGui headers
target_include_directories(colorReduction PUBLIC stb/include)                # stb headers
target_include_directories(colorReduction PUBLIC tinyfiledialogs/include)    # tinyfiledialogs headers
target_include_directories(colorReduction PUBLIC glm/include)                # glm headers
