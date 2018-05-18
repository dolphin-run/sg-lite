#ifndef GLSHADERPROGRAM_H
#define GLSHADERPROGRAM_H

#include <glm/glm.hpp>

class GLShaderProgram
{
public:
    void build(const char* vs, const char* fs, char const *const *attr);
    
    //use this shader
    void bind();

    //get uniform location
    unsigned uniformLocation(const char *name);

    //set uniform value
    void setInt(const char *name, int value) const;
    void setInt(unsigned pos, int value) const;
    
    void setFloat(const char *name, float value) const;
    void setFloat(unsigned pos, float value) const;
    
    void setVec2(const char *name, const glm::vec2 &value) const;
    void setVec2(unsigned pos, const glm::vec2 &value) const;
    void setVec2(const char *name, float x, float y) const;
    void setVec2(unsigned pos, float x, float y) const;
    
    void setVec3(const char *name, const glm::vec3 &value) const;
    void setVec3(unsigned pos, const glm::vec3 &value) const;
    void setVec3(const char *name, float x, float y, float z) const;
    void setVec3(unsigned pos, float x, float y, float z) const;
    
    void setVec4(const char *name, const glm::vec4 &value) const;
    void setVec4(unsigned pos, const glm::vec4 &value) const;
    void setVec4(const char *name, float x, float y, float z, float w);
    void setVec4(unsigned pos, float x, float y, float z, float w);

    void setMat4(const char *name, const glm::mat4 &mat) const;
    void setMat4(unsigned pos, const glm::mat4 &mat) const;
    void setMat4(unsigned pos, const float *mat4arr) const;

    void bindAttributeLocation(const char *name, int location);

private:
    void checkCompileError(unsigned shader);
    void checkLinkError(unsigned shader);

    unsigned m_id;
};
#endif
