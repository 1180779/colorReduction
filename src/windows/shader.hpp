
#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <iostream>

class shader 
{
public:

    shader(
        const char* vertexShaderC,
        const char* fragmentShaderC,
        bool fromFile = true);
    virtual ~shader();

    void use() const;

    void setb(const std::string& name, bool v) const;
    void set1i(const std::string& name, int v) const;
    void set1f(const std::string& name, float v) const;
    void set3f(const std::string& name, float v0, float v1, float v2) const;
    void set3f(const std::string& name, const glm::vec3& v) const;
    void set4f(const std::string& name, float v0, float v1, float v2, float v3) const;
    void setMatrix4fv(const std::string& name, const glm::mat4& trans) const;

private:
    GLuint program_;

    GLuint vertexShader_;
    GLuint fragmentShader_;
};

#endif