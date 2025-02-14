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

    /*void setb(const std::string& name, bool v) const;
    void set1i(const std::string& name, int v) const;
    void set1f(const std::string& name, float v) const;
    void set3f(const std::string& name, float v0, float v1, float v2) const;
    void set3f(const std::string& name, const glm::vec3& v) const;
    void set4f(const std::string& name, float v0, float v1, float v2, float v3) const;
    void setMatrix4fv(const std::string& name, const glm::mat4& trans) const;*/

private:
    GLuint program_;
};



#endif //COMPUTESHADER_HPP
