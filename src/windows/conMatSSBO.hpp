//
// Created by rdkgs on 2/20/2025.
//

#ifndef CONMATSSBO_HPP
#define CONMATSSBO_HPP

#include <cstring>
#include <vector>
#include <glad/glad.h>

#include "computeShader.hpp"
#include "../debug.hpp"

/* convolution matrix with SSBO */

class conMatSSBO {
public:
    conMatSSBO() = default;

    void init();
    void set(int n, int m, float *data);
    void destroy();

    void bindBuffer() const;
    void use(const computeShader& sh, int layoutN, int layoutM) const;
    static void unbindBuffer();

    GLuint SSBO;
    int n = 0, m = 0; /* matrix dimentions */
    std::vector<float> mat = {}; /* matrix data */
};

//////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

inline void conMatSSBO::init() {
    glGenBuffers(1, &SSBO);
}

inline void conMatSSBO::set(int n, int m, float* data) {
    this->n = n;
    this->m = m;
    mat.resize(n * m);
    memcpy(mat.data(), data, n * m * sizeof(float));
    bindBuffer();
    glBufferData(GL_SHADER_STORAGE_BUFFER,
        static_cast<GLsizeiptr>(n * m * sizeof(float)),
        mat.data(),
        GL_STATIC_DRAW);
    glXCheckError();
}

inline void conMatSSBO::destroy() {
    glDeleteBuffers(1, &SSBO);
}

inline void conMatSSBO::bindBuffer() const {
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, SSBO);
}

inline void conMatSSBO::use(const computeShader& sh, int layoutN, int layoutM) const {
    bindBuffer();
    sh.setl1i(layoutN, n);
    sh.setl1i(layoutM, m);
}

inline void conMatSSBO::unbindBuffer() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

#endif //CONMATSSBO_HPP
