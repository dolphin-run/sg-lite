
#include "view.h"
#include <functional>
#include <thread>
#include <chrono>

bool View::g_glfwInitialized = false;
std::unordered_map<GLFWwindow*, View*> View::g_glfwView;
void View::InitializeGlfw()
{
    // glfw: initialize and configure
    if (GLFW_TRUE == glfwInit())
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

        g_glfwInitialized = true;
    }
}

void View::FreeGlfw()
{
    glfwTerminate();
}

void View::windowSizeCallback(GLFWwindow* window, int width, int height)
{
    if (g_glfwView.count(window))
        g_glfwView.at(window)->resizeEvent(width, height);
}

void View::windowScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (g_glfwView.count(window))
        g_glfwView.at(window)->CameraScale(yoffset>0);
}

void View::windowKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    int xmove = 0;
    int ymove = 0;
    if(action == GLFW_PRESS)
        switch (key)
        {
        case GLFW_KEY_LEFT:
            xmove += 10;
            break;
        case GLFW_KEY_RIGHT:
            xmove -= 10;
            break;
        case GLFW_KEY_UP:
            ymove += 10;
            break;
        case GLFW_KEY_DOWN:
            ymove -= 10;
            break;
        }

    if (g_glfwView.count(window))
        g_glfwView.at(window)->CameraMove(xmove, ymove);
}

void View::exec()
{
    while (g_glfwView.size())
    {
        for (auto it : g_glfwView)
        {
            if (glfwWindowShouldClose(it.first))
            {
                it.second->destroy();
                break;
            }
            glfwMakeContextCurrent(it.first);
            it.second->renderFrame();
            glfwSwapBuffers(it.first);
            glfwPollEvents();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    }
}

void View::resizeEvent(int width, int height)
{
    m_width = width;
    m_height = height;
    m_camera.setViewSize(m_width, m_height);
}

View::View()
{
    m_camera.setViewSize(800, 600);
}


View::~View()
{
}

bool View::create(const char* title)
{
    if (!g_glfwInitialized) return false;

    //The window and its OpenGL context are created with a single call to glfwCreateWindow, 
    //which returns a handle to the created combined window and context object
    GLFWwindow *window = NULL;
    if (g_glfwView.size()) window = g_glfwView.begin()->first;
    m_window = glfwCreateWindow(800, 600, title, NULL, window);
    if (m_window == NULL)
    {
        printf("Failed to create GLFW window\n");
        return false;
    }
    glfwMakeContextCurrent(m_window);

    glfwSetFramebufferSizeCallback(m_window, windowSizeCallback); 
    glfwSetKeyCallback(m_window, windowKeyCallback);
    glfwSetScrollCallback(m_window, windowScrollCallback);

    g_glfwView.insert(std::make_pair(m_window, this));
}

void View::destroy()
{
    if (m_window)
    {
        g_glfwView.erase(m_window);
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }
}

float View::width() const
{
    return m_width;
}

float View::height() const
{
    return m_height;
}

float View::pixelRatio() const
{
    return 1.f;
}

void View::CameraMove(int x, int y)
{
    m_camera.move(x, y);
}

void View::CameraScale(bool zoom)
{
    if (zoom)
    {
        m_camera.scale(1.01f);
    }
    else
    {
        m_camera.scale(1.f / 1.01f);
    }
}