
#ifndef SHADER_HPP
#define SHADER_HPP

#include "shaderBase.hpp"

class shader : public shaderBase
{
public:

    shader(
        const char* vertexShaderC,
        const char* fragmentShaderC,
        bool fromFile = true);
    ~shader();

    void use() const;
};

#endif