
file(GLOB_RECURSE SOURCE_IMGUI
        ./src/*.h ./src/*.hpp
        ./src/*.c ./src/*.cpp)

add_library(imGui ${SOURCE_IMGUI})
target_include_directories(imGui PUBLIC ${CMAKE_SOURCE_DIR}/glfw/include)
