//
// Created on 14.02.2025.
//

#ifndef COMPUTESHADER_HPP
#define COMPUTESHADER_HPP

#include <glad/glad.h>

#include "shaderBase.hpp"

class computeShader : public shaderBase {
public:

    explicit computeShader(
        const char* shaderC,
        bool fromFile = true);
    ~computeShader();

    void use() const;

private:
    GLuint program_;
};

#endif //COMPUTESHADER_HPP
