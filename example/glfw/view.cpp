
#include "view.h"
#include <functional>
#include <thread>
#include <chrono>
#include "sgviewcamera.h"

bool View::g_glfwInitialized = false;
std::unordered_map<GLFWwindow*, View*> View::g_glfwView;
void View::InitializeGlfw()
{
    // glfw: initialize and configure
    if (GLFW_TRUE == glfwInit())
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_SAMPLES, 4);

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
        g_glfwView.at(window)->setSize(width, height);
}
void View::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    if (g_glfwView.count(window))
    {
        auto view = g_glfwView.at(window);
        view->setSizePx(width, height);
    }
}

void View::windowScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
{
    if (g_glfwView.count(window))
        g_glfwView.at(window)->viewCamera()->scale(yoffset > 0 ? 1.1f : 1.f / 1.1f);
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
        g_glfwView.at(window)->viewCamera()->move(xmove, ymove);
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

View::View()
{
    SGView::setSize(800, 600);
}


View::~View()
{
}

bool View::create(const char* title)
{
    if (!g_glfwInitialized) return false;

    //The window and its OpenGL context are created with a single call to glfwCreateWindow, 
    //which returns a handle to the created combined window and context object
    GLFWwindow *firstwindow = NULL;
    if (g_glfwView.size()) firstwindow = g_glfwView.begin()->first;
    m_window = glfwCreateWindow(800, 600, title, NULL, firstwindow);
    if (m_window == NULL)
    {
        printf("Failed to create GLFW window\n");
        return false;
    }
    glfwMakeContextCurrent(m_window);

    glfwSetWindowSizeCallback(m_window, windowSizeCallback); 
    glfwSetKeyCallback(m_window, windowKeyCallback);
    glfwSetScrollCallback(m_window, windowScrollCallback);
    glfwSetFramebufferSizeCallback(m_window, framebufferSizeCallback);

    //force to update pixel-size of the window
    int cwid,chei;
    glfwGetFramebufferSize(m_window, &cwid, &chei);
    SGView::setSizePx(cwid,chei);

    g_glfwView.insert(std::make_pair(m_window, this));

    return true;
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