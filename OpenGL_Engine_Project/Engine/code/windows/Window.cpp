#include "window.h"
#include "../common/common.h"
#include "../common/enums.h"

using namespace application;

application::Window::Window(std::string_view title, int width_, int height_)
    : width(width_), height(height_)
{   
    eventResizeWindow += [this](GLFWwindow* window, int width_, int height_)
    {
        glViewport(0, 0, width, height);
        width = width_;
        height = height_;
    };

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(width, height, title.data(), NULL, NULL);

    if (!window)
    {
        //glfwTerminate();
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Enable vsync

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        //glfwTerminate();
    }

    glViewport(0, 0, width, height);
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

void application::Window::on_resize_window(int width, int height)
{
    eventResizeWindow(window, width, height);
}

void application::Window::on_mouse_move(double xpos, double ypos)
{
    eventMouseMove(window, xpos, ypos);
}

void application::Window::on_mouse_button_action(int button, int action, int mode)
{
    eventMouseAction(window, button, action, mode);
}

void application::Window::on_keyboard_action(int keycode, int scancode, int action, int mode)
{
    eventKeyboardAction(window, keycode, scancode, action, mode);
}
