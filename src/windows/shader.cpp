#include "shader.hpp"

#include <iostream>
#include <string>
#include <fstream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

GLuint compileShader(const char* source, GLenum shaderType);
GLuint linkProgram(
    GLuint vertexShader,
    GLuint fragmentShader);

shader::shader(
    const char* vertexShaderC,
    const char* fragmentShaderC,
    bool fromFile)
{
    if (!fromFile) {
        vertexShader_ = ::compileShader(vertexShaderC, GL_VERTEX_SHADER);
        fragmentShader_ = ::compileShader(fragmentShaderC, GL_FRAGMENT_SHADER);
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

        vertexShader_ = ::compileShader(vertexShaderC, GL_VERTEX_SHADER);
        fragmentShader_ = ::compileShader(fragmentShaderC, GL_FRAGMENT_SHADER);
    }

    program_ = ::linkProgram(
        vertexShader_,
        fragmentShader_);

    glDeleteShader(vertexShader_);
    glDeleteShader(fragmentShader_);
}

shader::~shader()
{
    glDeleteProgram(program_);
}

void shader::use() const
{
    glUseProgram(program_);
}

void shader::setb(const std::string& name, bool b) const
{
    glUniform1i(glGetUniformLocation(program_, name.c_str()),
        static_cast<int>(b));
}

void shader::set1i(const std::string& name, int v) const
{
    glUniform1i(glGetUniformLocation(program_, name.c_str()), v);
}

void shader::set1f(const std::string& name, float v) const
{
    glUniform1f(glGetUniformLocation(program_, name.c_str()), v);
}

void shader::set3f(const std::string& name, float v0, float v1, float v2) const
{
    glUniform3f(glGetUniformLocation(program_, name.c_str()), v0, v1, v2);
}

void shader::set3f(const std::string& name, const glm::vec3& v) const
{
    glUniform3f(glGetUniformLocation(program_, name.c_str()), v.x, v.y, v.z);
}

void shader::set4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
    glUniform4f(glGetUniformLocation(program_, name.c_str()), v0, v1, v2, v3);
}

void shader::setMatrix4fv(const std::string& name, const glm::mat4& trans) const
{
    glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
}

// ------------------------------------------------
// implementation

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