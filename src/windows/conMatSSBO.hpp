//
// Created by rdkgs on 2/20/2025.
//

#ifndef CONMATSSBO_HPP
#define CONMATSSBO_HPP

#include <glad/glad.h>

#include "computeShader.hpp"
#include "mat.hpp"
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
    mat mat;
};

//////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

inline void conMatSSBO::init() {
    glGenBuffers(1, &SSBO);
}

inline void conMatSSBO::set(int n, int m, float* data) {
    mat.set(n, m, data);
    bindBuffer();
    glBufferData(GL_SHADER_STORAGE_BUFFER,
        static_cast<GLsizeiptr>(n * m * sizeof(float)),
        mat.getData(),
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
    sh.setl1i(layoutN, mat.getN());
    sh.setl1i(layoutN + 1, mat.getN() / 2);
    sh.setl1i(layoutM, mat.getM());
    sh.setl1i(layoutM + 1, mat.getM() / 2);
}

inline void conMatSSBO::unbindBuffer() {
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

#endif //CONMATSSBO_HPP
