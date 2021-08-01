#include "Window.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>

GLFWwindow* Window::window;
int Window::width;
int Window::height;

int Window::createWindow(int width, int height, const char* title)
{
    /* Initialize the library */
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, title, NULL, NULL);
    if (!window)
    {
        glfwTerminate();
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
    }

    glViewport(0, 0, width, height);
    Window::width = width;
    Window::height = height;
    return 0;
}

void Window::terminate()
{
    glfwTerminate();
}

void Window::swapBuffer()
{
    glfwSwapBuffers(window);
}

bool Window::isWindowClose()
{
    return !glfwWindowShouldClose(window);
}

void Window::closeWindow(bool close)
{
    glfwSetWindowShouldClose(window, close);
}

void Window::setCursorMode(int mode) {
    glfwSetInputMode(window, GLFW_CURSOR, mode);
}