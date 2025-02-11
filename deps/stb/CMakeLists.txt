
file(GLOB_RECURSE SOURCE_STB
        ./src/*.h ./src/*.hpp
        ./src/*.c ./src/*.cpp)

add_library(stb ${SOURCE_STB})
