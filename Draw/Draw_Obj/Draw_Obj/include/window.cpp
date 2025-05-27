#include "window.h"
#include <iostream>

// init 및 구성
static Window* activeWindowInstance = nullptr;
// ----------------------------------------------------------------------
Window::Window(unsigned int width, unsigned int height, const char* title)
    : width(width), height(height), title(title), camera(nullptr), lastX(width / 2.0f), lastY(height / 2.0f)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;        
        glfwTerminate();
        exit(-1);
    }

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initailize GLAD" << std::endl;
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    activeWindowInstance = this;
}
// ----------------------------------------------------------------------
Window::~Window()
{
    glfwDestroyWindow(window);
    glfwTerminate();
}
// ----------------------------------------------------------------------
GLFWwindow* Window::GetGLFWWindow() const
{
    return window;
}
// ----------------------------------------------------------------------
void Window::SetCamera(Camera* cam)
{
    camera = cam;
}
// ----------------------------------------------------------------------
void Window::SetDeltaTime(float delta)
{
    deltaTime = delta;
}
// ----------------------------------------------------------------------
// utill
void Window::ProcessInput()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (!camera) return;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
}
// ----------------------------------------------------------------------
void Window::InitCallbacks()
{
    glfwSetFramebufferSizeCallback(window, FramebufferSizeCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}
// ----------------------------------------------------------------------
void Window::FramebufferSizeCallback(GLFWwindow* win, int width, int height)
{
    glViewport(0, 0, width, height);
}
// ----------------------------------------------------------------------
void Window::MouseCallback(GLFWwindow* win, double xpos, double ypos)
{
    if (!activeWindowInstance || !activeWindowInstance->camera) return;

    Window* self = activeWindowInstance;

    float x = static_cast<float>(xpos);
    float y = static_cast<float>(ypos);

    if (self->firstMouse)
    {
        self->lastX = x;
        self->lastY = y;
        self->firstMouse = false;
    }

    float xoffset = x - self->lastX;
    float yoffset = self->lastY - y; // reversed: y-coordinates go from bottom to top

    self->lastX = x;
    self->lastY = y;

    self->camera->ProcessMouseMovement(xoffset, yoffset);
}
// ----------------------------------------------------------------------
void Window::ScrollCallback(GLFWwindow* win, double xoffset, double yoffset)
{
    if (!activeWindowInstance || !activeWindowInstance->camera) return;
    activeWindowInstance->camera->ProcessMouseScroll(static_cast<float>(yoffset));
}
// ----------------------------------------------------------------------
