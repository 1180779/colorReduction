//
// Created on 14.02.2025.
//

#include "computeShader.hpp"
#include <iostream>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.hpp"

static GLuint compileShader(const char* source, GLenum shaderType);
static GLuint linkProgram(GLuint computeShader);

computeShader::computeShader(const char * shaderC, bool fromFile)
{
    GLuint computeShader = 0;
    if (!fromFile) {
        computeShader = ::compileShader(shaderC, GL_COMPUTE_SHADER);
    }
    else {
        // read from file first
        std::ifstream v(shaderC);
        std::string vStr((std::istreambuf_iterator<char>(v)),
                 std::istreambuf_iterator<char>());
        shaderC = vStr.c_str();

        computeShader = ::compileShader(shaderC, GL_COMPUTE_SHADER);
    }

    program_ = ::linkProgram(computeShader);

    glDeleteShader(computeShader);
}

computeShader::~computeShader()
{
    glDeleteProgram(program_);
}

void computeShader::use() const
{
    glUseProgram(program_);
}

GLuint compileShader(const char* source, GLenum shaderType) {
    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Check for compilation errors
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, nullptr, infoLog);
        std::cerr << "Shader Compilation Failed:\n" << infoLog << std::endl;
    }

    return shader;
}

GLuint linkProgram(GLuint computeShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, computeShader);
    glLinkProgram(program);

    // Check for linking errors
    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, nullptr, infoLog);
        std::cerr << "Program Linking Failed:\n" << infoLog << std::endl;
    }

    return program;
}
