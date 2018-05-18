#ifndef SGGLWRAPPER_H
#define SGGLWRAPPER_H

//#define SG_GL_GLAD

#ifdef SG_GL_GLAD
#include "glad/glad.h"
#else
#include "GL/glew.h"
#endif // SG_GL_GLAD

bool sgGLInit();


#endif // SGGLWRAPPER_H
