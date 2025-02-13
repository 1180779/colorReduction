//
// Created on 13.02.2025.
//

#ifndef DEBUG_HPP
#define DEBUG_HPP

#define glXCheckError()   \
    do { \
        GLenum err; \
        while((err = glGetError()) != GL_NO_ERROR) { \
            fprintf(stderr, "error %04X, FILE: %s, LINE: %d\n", err, __FILE__, __LINE__); \
        } \
    } while(0)

#endif //DEBUG_HPP
