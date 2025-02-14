//
// Created on 14.02.2025.
//

#ifndef COMPUTESHADER_HPP
#define COMPUTESHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <string>

class computeShader {
public:

    explicit computeShader(
        const char* shaderC,
        bool fromFile = true);
    virtual ~computeShader();

    void use() const;

    void setb(GLint location, bool v) const;
    void set1i(const std::string& name, GLint v) const;
    void set1f(GLint location, GLfloat v) const;
    void set3f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) const;
    void set3f(GLint location, const glm::vec3& v) const;
    void set4f(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) const;
    void setMatrix4fv(GLint location, const glm::mat4& trans) const;

private:
    GLuint program_;
};



#endif //COMPUTESHADER_HPP
