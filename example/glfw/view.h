#ifndef VIEW_H
#define VIEW_H

#include <unordered_map>
#include "sgview.h"
#include <GLFW/glfw3.h>


class View : public SGView
{
public:
    explicit View();
    ~View();

    bool create(const char* title);
    void destroy();

    static void exec();
    static void windowSizeCallback(GLFWwindow* window, int width, int height); 
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void windowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void windowScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    static void InitializeGlfw();
    static void FreeGlfw();

private:
    GLFWwindow* m_window;

    int m_widthPx = 800;
    int m_heightPx = 600;

    static bool g_glfwInitialized;
    static std::unordered_map<GLFWwindow*, View*> g_glfwView;
};

#endif // VIEW_H
