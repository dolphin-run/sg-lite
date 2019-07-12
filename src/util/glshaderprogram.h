#ifndef GLSHADERPROGRAM_H
#define GLSHADERPROGRAM_H

#include <glm/glm.hpp>
#include <glad/glad.h>

#include <string>
#include <iostream>


class GLShaderProgram
{
public:
    unsigned int ID;

    void setShader(const char* vShaderCode, const char* fShaderCode);
    // activate the shader
    // ------------------------------------------------------------------------
    void bind();
    // utility uniform functions
    // ------------------------------------------------------------------------
    void setBool(const std::string &name, bool value) const;
    // ------------------------------------------------------------------------
    void setInt(const std::string &name, int value) const;
    // ------------------------------------------------------------------------
    void setFloat(const std::string &name, float value) const;
    // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w);
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const;
    void setMat4(GLint pos, const glm::mat4 &mat) const;

    GLint uniformLocation(const std::string &name);
    void bindAttributeLocation(const char *name, int location);

private:
    // utility function for checking shader compilation/linking errors.
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned shader, std::string type);
};
#endif
