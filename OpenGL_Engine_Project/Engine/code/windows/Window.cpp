#include "Window.h"
#include "../common/enums.h"

GLFWwindow* Window::window;
int Window::width;
int Window::height;

int Window::initialize(int width, int height, const char* title)
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

void Window::swap_buffers()
{
    glfwSwapBuffers(window);
}

bool Window::isShouldClose()
{
    return glfwWindowShouldClose(window);
}

void Window::setShouldClose(bool close)
{
    glfwSetWindowShouldClose(window, close);
}

void Window::setCursorMode(CursorMode mode) {
    glfwSetInputMode(window, GLFW_CURSOR, (int)mode);
}