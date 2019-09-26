#include "glshaderprogram.h"
#include "sgglwrapper.h"
#include <cstdio>

void GLShaderProgram::build(const char* vs, const char* fs, char const *const *attr)
{
    unsigned vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vs, NULL);
    glCompileShader(vertex);
    checkCompileError(vertex);

    // fragment Shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fs, NULL);
    glCompileShader(fragment);
    checkCompileError(fragment);
    

    // link
    m_id = glCreateProgram();
    glAttachShader(m_id, vertex);
    glAttachShader(m_id, fragment);

    //bind attributes
    for (int i = 0; attr[i]; ++i) {
        if (*attr[i])
            bindAttributeLocation(attr[i], i);
    }

    glLinkProgram(m_id);
    checkLinkError(m_id);

    // delete the shaders after linking
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void GLShaderProgram::bind()
{
    glUseProgram(m_id);
}


unsigned GLShaderProgram::uniformLocation(const char *name)
{
    return glGetUniformLocation(m_id, name);
}

void GLShaderProgram::setInt(const char* name, int value) const
{
    glUniform1i(glGetUniformLocation(m_id, name), value);
}
void GLShaderProgram::setInt(unsigned pos, int value) const
{
    glUniform1i(pos, value);
}

void GLShaderProgram::setFloat(const char* name, float value) const
{
    glUniform1f(glGetUniformLocation(m_id, name), value);
}
void GLShaderProgram::setFloat(unsigned pos, float value) const
{
    glUniform1f(pos, value);
}


void GLShaderProgram::setVec2(const char*name, const glm::vec2 &value) const
{
    glUniform2fv(glGetUniformLocation(m_id, name), 1, &value[0]);
}
void GLShaderProgram::setVec2(unsigned pos, const glm::vec2 &value) const
{
    glUniform2fv(pos, 1, &value[0]);
}
void GLShaderProgram::setVec2(const char*name, float x, float y) const
{
    glUniform2f(glGetUniformLocation(m_id, name), x, y);
}
void GLShaderProgram::setVec2(unsigned pos, float x, float y) const
{
    glUniform2f(pos, x, y);
}


void GLShaderProgram::setVec3(const char* name, const glm::vec3 &value) const
{
    glUniform3fv(glGetUniformLocation(m_id, name), 1, &value[0]);
}
void GLShaderProgram::setVec3(unsigned pos, const glm::vec3 &value) const
{
    glUniform3fv(pos, 1, &value[0]);
}
void GLShaderProgram::setVec3(const char*name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(m_id, name), x, y, z);
}
void GLShaderProgram::setVec3(unsigned pos, float x, float y, float z) const
{
    glUniform3f(pos, x, y, z);
}


void GLShaderProgram::setVec4(const char*name, const glm::vec4 &value) const
{
    glUniform4fv(glGetUniformLocation(m_id, name), 1, &value[0]);
}
void GLShaderProgram::setVec4(unsigned pos, const glm::vec4 &value) const
{
    glUniform4fv(pos, 1, &value[0]);
}
void GLShaderProgram::setVec4(const char*name, float x, float y, float z, float w)
{
    glUniform4f(glGetUniformLocation(m_id, name), x, y, z, w);
}
void GLShaderProgram::setVec4(unsigned pos, float x, float y, float z, float w)
{
    glUniform4f(pos, x, y, z, w);
}

void GLShaderProgram::setMat4(const char*name, const glm::mat4 &mat) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_id, name), 1, GL_FALSE, &mat[0][0]);
}
void GLShaderProgram::setMat4(unsigned pos, const glm::mat4 & mat) const
{
    glUniformMatrix4fv(pos, 1, GL_FALSE, &mat[0][0]);
}
void GLShaderProgram::setMat4(unsigned pos, const float *mat4arr) const
{
    glUniformMatrix4fv(pos, 1, GL_FALSE, mat4arr);
}

void GLShaderProgram::bindAttributeLocation(const char * name, int location)
{
    glBindAttribLocation(m_id, location, name);
}

//checking shader compilation/linking errors.
void GLShaderProgram::checkCompileError(unsigned shader)
{
    GLint ok;
    GLchar info[1024];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok)
    {
        glGetShaderInfoLog(shader, 1024, NULL, info);
        printf("[ERROR] gl compilation error: %s\n", info);
    }
}

void GLShaderProgram::checkLinkError(unsigned shader)
{
    GLint ok;
    GLchar info[1024];

    glGetProgramiv(shader, GL_LINK_STATUS, &ok);
    if (!ok)
    {
        glGetProgramInfoLog(shader, 1024, NULL, info);
        printf("[ERROR] gl linking error: %s\n", info);
    }
}