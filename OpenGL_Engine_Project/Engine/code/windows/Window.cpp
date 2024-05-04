#include "window.h"
#include "../common/common.h"
#include "../common/enums.h"
#include "../render/rnd_render_system.h"
#include "../common/timer.hpp"

using namespace windows;

windows::Window::Window(std::string_view title, glm::ivec2 size_)
    : size(size_)
{   
    /* Create a windowed mode window */
    id_.id_ = glfwCreateWindow(size.x, size.y, title.data(), NULL, NULL);

    ASSERT_MSG(id_, "Window didn't create!");

    _render_context.init(id_);

    rnd::get_system().viewport({ glm::zero<glm::ivec2>(), size });
}

windows::Window::~Window()
{
    //glfwDestroyWindow
}

void Window::on_update()
{
    _render_context.swap_buffers();
}

bool Window::is_shutdown() const
{
    return glfwWindowShouldClose(id_);
}

void Window::shutdown(bool close)
{
    glfwSetWindowShouldClose(id_, close);
}

void Window::set_cursor_mode(CursorMode mode) {
    glfwSetInputMode(id_, GLFW_CURSOR, (int)mode);
}

void Window::update_frame()
{
    double currentTime = Timer::now();
    delta = currentTime - lastTime;
    lastTime = currentTime;
}

void windows::Window::on_resize_window(int width, int height)
{
    size = { width, height };
    rnd::get_system().viewport({ glm::zero<glm::ivec2>(), size});
    eventResizeWindow(*this, width, height);
}

void windows::Window::set_logo(res::ImageRef logo, res::ImageRef logo_small)
{
    if (logo->channels() != 4) {
        egLOG("window/logo", "Logo should be rgba image");
        return;
    }

    GLFWimage images[2];
    images[0].pixels = logo->data();
    images[0].width = logo->size().x;
    images[0].height = logo->size().y;

    if (logo_small) {
        images[1].pixels = logo_small->data();
        images[1].width = logo_small->size().x;
        images[1].height = logo_small->size().y;
        glfwSetWindowIcon(id_, 2, images);
        return;
    }

    glfwSetWindowIcon(id_, 1, images);
}
