//
// Created on 13.02.2025.
//

#ifndef DEBUG_HPP
#define DEBUG_HPP

#include <iostream>
#include <ios>

#define glXCheckError()   \
    do { \
        GLenum err = glGetError(); \
        while(err != GL_NO_ERROR) { \
            std::cerr << "error: " << std::hex << err << std::dec << ", "; \
            std::cerr << "FILE: " << __FILE__ << ", LINE: " << static_cast<int>(__LINE__) << std::endl; \
            err = glGetError(); \
        } \
    } while(0)

#endif //DEBUG_HPP
