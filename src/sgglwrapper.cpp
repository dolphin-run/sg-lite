#include "sgglwrapper.h"
#include "sgglwrapper.h"
#include <cstdio>

#ifdef SG_GL_GLAD
#include "glad/glad.c"
#endif

bool sgGLInit()
{
#ifdef SG_GL_GLAD
    int ret = gladLoadGL();
    if (!ret)
    {
        printf("Failed to initialize GLAD\n");
    }
    printf("Status: Using GLAD, GL%d.%d\n", GLVersion.major, GLVersion.minor);
    return ret;
#else
    GLenum ret = glewInit();
    if (GLEW_OK != ret)
    {
        /* Problem: glewInit failed, something is seriously wrong. */
        printf("Failed to initialize GLEW: %s\n", glewGetErrorString(ret));
        return false;
    }
    printf("Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
    return true;
#endif
}
