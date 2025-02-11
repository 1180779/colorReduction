
file(GLOB_RECURSE SOURCE_GLAD
        *.h *.hpp
        *.cpp *.c)

include_directories(include)
add_library(glad ${SOURCE_GLAD})
