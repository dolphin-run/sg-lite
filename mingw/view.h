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


    virtual int width() const override;
    virtual int height() const override;
    virtual float pixelRatio() const override;

    void CameraMove(int x, int y);
    void CameraScale(bool zoom);

    static void exec();
    static void windowSizeCallback(GLFWwindow* window, int width, int height); 
    static void windowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void windowScrollCallback(GLFWwindow* window, double xoffset, double yoffset);

    static void InitializeGlfw();
    static void FreeGlfw();

protected:
    void resizeEvent(int width, int height);

private:
    GLFWwindow* m_window;

    int m_width = 800;
    int m_height = 600;

    static bool g_glfwInitialized;
    static std::unordered_map<GLFWwindow*, View*> g_glfwView;
};

#endif // VIEW_H
