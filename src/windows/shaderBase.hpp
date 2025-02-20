//
// Created by rdkgs on 2/20/2025.
//

#ifndef SHADERBASE_HPP
#define SHADERBASE_HPP

#include <glad/glad.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>

class shaderBase {
public:
    shaderBase() = default;
    ~shaderBase() = default;

    void setb(const std::string& name, bool b) const;
    void set1i(const std::string& name, int v) const;
    void set1f(const std::string& name, float v) const;
    void set3f(const std::string& name, float v0, float v1, float v2) const;
    void set3f(const std::string& name, const glm::vec3& v) const;
    void set4f(const std::string& name, float v0, float v1, float v2, float v3) const;
    void setMatrix4fv(const std::string& name, const glm::mat4& trans) const;

    void setlb(GLint location, bool b) const;
    void setl1i(GLint location, int v) const;
    void setl1f(GLint location, float v) const;
    void setl3f(GLint location, float v0, float v1, float v2) const;
    void setl3f(GLint location, const glm::vec3& v) const;
    void setl4f(GLint location, float v0, float v1, float v2, float v3) const;
    void setlMatrix4fv(GLint location, const glm::mat4& trans) const;

protected:
    GLuint program_ = 0;
};

///////////////////////////////////////////////////////////////////
/// implementation
//////////////////////////////////////////////////////////////////

inline void shaderBase::setb(const std::string& name, bool b) const
{
    glUniform1i(glGetUniformLocation(program_, name.c_str()),
        static_cast<int>(b));
}

inline void shaderBase::set1i(const std::string& name, int v) const
{
    glUniform1i(glGetUniformLocation(program_, name.c_str()), v);
}

inline void shaderBase::set1f(const std::string& name, float v) const
{
    glUniform1f(glGetUniformLocation(program_, name.c_str()), v);
}

inline void shaderBase::set3f(const std::string& name, float v0, float v1, float v2) const
{
    glUniform3f(glGetUniformLocation(program_, name.c_str()), v0, v1, v2);
}

inline void shaderBase::set3f(const std::string& name, const glm::vec3& v) const
{
    glUniform3f(glGetUniformLocation(program_, name.c_str()), v.x, v.y, v.z);
}

inline void shaderBase::set4f(const std::string& name, float v0, float v1, float v2, float v3) const
{
    glUniform4f(glGetUniformLocation(program_, name.c_str()), v0, v1, v2, v3);
}

inline void shaderBase::setMatrix4fv(const std::string& name, const glm::mat4& trans) const
{
    glUniformMatrix4fv(glGetUniformLocation(program_, name.c_str()), 1, GL_FALSE, glm::value_ptr(trans));
}



inline void shaderBase::setlb(GLint location, bool b) const
{
    glUniform1i(location,  static_cast<int>(b));
}

inline void shaderBase::setl1i(GLint location, int v) const
{
    glUniform1i(location, v);
}

inline void shaderBase::setl1f(GLint location, float v) const
{
    glUniform1f(location, v);
}

inline void shaderBase::setl3f(GLint location, float v0, float v1, float v2) const
{
    glUniform3f(location, v0, v1, v2);
}

inline void shaderBase::setl3f(GLint location, const glm::vec3& v) const
{
    glUniform3f(location, v.x, v.y, v.z);
}

inline void shaderBase::setl4f(GLint location, float v0, float v1, float v2, float v3) const
{
    glUniform4f(location, v0, v1, v2, v3);
}

inline void shaderBase::setlMatrix4fv(GLint location, const glm::mat4& trans) const
{
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(trans));
}

#endif //SHADERBASE_HPP
