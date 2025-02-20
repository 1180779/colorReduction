#include "shader.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static GLuint compileShader(const char* source, GLenum shaderType);
static GLuint linkProgram(
    GLuint vertexShader,
    GLuint fragmentShader);

shader::shader(
    const char* vertexShaderC,
    const char* fragmentShaderC,
    bool fromFile)
{
    GLuint vertexShader;
    GLuint fragmentShader;
    if (!fromFile) {
        vertexShader = ::compileShader(vertexShaderC, GL_VERTEX_SHADER);
        fragmentShader = ::compileShader(fragmentShaderC, GL_FRAGMENT_SHADER);
    }
    else {
        // read from file first
        std::ifstream v(vertexShaderC);
        std::string vStr((std::istreambuf_iterator<char>(v)),
                 std::istreambuf_iterator<char>());
        vertexShaderC = vStr.c_str();

        std::ifstream f(fragmentShaderC);
        std::string fStr((std::istreambuf_iterator<char>(f)),
                 std::istreambuf_iterator<char>());
        fragmentShaderC = fStr.c_str();

        vertexShader = ::compileShader(vertexShaderC, GL_VERTEX_SHADER);
        fragmentShader = ::compileShader(fragmentShaderC, GL_FRAGMENT_SHADER);
    }

    program_ = ::linkProgram(
        vertexShader,
        fragmentShader);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

shader::~shader()
{
    glDeleteProgram(program_);
}

void shader::use() const
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

GLuint linkProgram(
    GLuint vertexShader,
    GLuint fragmentShader)
{
    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
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