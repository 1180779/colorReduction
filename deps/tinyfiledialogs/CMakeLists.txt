
file(GLOB_RECURSE SOURCE_TINYFILEDIALOGS
        ./src/*.h ./src/*.hpp
        ./src/*.c ./src/*.cpp)

add_library(tinyfiledialogs ${SOURCE_TINYFILEDIALOGS})
