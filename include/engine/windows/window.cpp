#include "window.h"
#include "../common/common.h"
#include "../common/enums.h"

using namespace windows;

windows::Window::Window(std::string_view title, int width, int height)
    : width_(width), height_(height)
{   
    /* Create a windowed mode window and its OpenGL context */
    id_.id_ = glfwCreateWindow(width_, height_, title.data(), NULL, NULL);

    if (!id_)
    {
        ASSERT_FAIL("window didnt create!");
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(id_);

    // Enable vsync
    glfwSwapInterval(1); 

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        ASSERT_FAIL("glad didnt loaded in this process!");
    }

    glViewport(0, 0, width_, height_);
}

windows::Window::~Window()
{
    //glfwDestroyWindow
}

void Window::swap_buffers()
{
    glfwSwapBuffers(id_);
    ++current_frame;
}

bool Window::is_should_close()
{
    return glfwWindowShouldClose(id_);
}

void Window::set_should_close(bool close)
{
    glfwSetWindowShouldClose(id_, close);
}

void Window::set_cursor_mode(CursorMode mode) {
    glfwSetInputMode(id_, GLFW_CURSOR, (int)mode);
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

void windows::Window::on_resize_window(int width, int height)
{
    glViewport(0, 0, width, height);
    width_ = width;
    height_ = height;
    eventResizeWindow(*this, width, height);
}

void windows::Window::on_mouse_move(double xpos, double ypos)
{
    eventMouseMove(*this, xpos, ypos);
}

void windows::Window::on_mouse_button_action(int button, int action, int mode)
{
    eventMouseAction(*this, button, action, mode);
}

void windows::Window::on_keyboard_action(int keycode, int scancode, int action, int mode)
{
    eventKeyboardAction(*this, keycode, scancode, action, mode);
}
