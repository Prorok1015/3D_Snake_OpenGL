#include "window.h"
#include "../common/enums.h"

GLFWwindow* Window::window;
int Window::width;
int Window::height;
float Window::lastTime = 0.f;
float Window::delta = 0.f;
unsigned Window::current_frame = 0;

void window_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
    Window::width = width;
    Window::height = height;
}

int Window::initialize(const char* title, int width, int height)
{
    /* Initialize the library */
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, title, NULL, NULL);

    glfwSwapInterval(1); // Enable vsync
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

    glfwSetWindowSizeCallback(window, window_size_callback);

    return 0;
}

void Window::terminate()
{
    glfwTerminate();
}

void Window::swap_buffers()
{
    glfwSwapBuffers(window);
    ++current_frame;
}

bool Window::is_should_close()
{
    return glfwWindowShouldClose(window);
}

void Window::set_should_close(bool close)
{
    glfwSetWindowShouldClose(window, close);
}

void Window::set_cursor_mode(CursorMode mode) {
    glfwSetInputMode(window, GLFW_CURSOR, (int)mode);
}

float Window::current_time()
{
    return (float)glfwGetTime();
}

void Window::update_frame()
{
    float currentTime = Window::current_time();
    delta = currentTime - lastTime;
    lastTime = currentTime;
}
