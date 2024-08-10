#include "window_system.h"
#include "window.h"
#include "../input/inp_input_system.h"
#include <rnd_gl_render_context.h>

windows::WindowSystem* p_wnd_system = nullptr;

windows::WindowSystem& windows::get_system()
{
    ASSERT_MSG(p_wnd_system, "Window system is nullptr!");
    return *p_wnd_system;
}

namespace {
    void window_size_callback(GLFWwindow* window, int width, int height) {
        auto& wndCreator = wnd::get_system();
        auto wnd = wndCreator.find_window({ window });
        if (auto swnd = wnd.lock()) {
            swnd->on_resize_window(width, height);
        }
    }

    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
        auto& wndCreator = wnd::get_system();

        inp::InputSystem& inpSys = inp::get_system();
        inpSys.mouse.on_mouse_move(xpos, ypos);
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
        auto& wndCreator = wnd::get_system();

        inp::InputSystem& inpSys = inp::get_system();
        inpSys.mouse.on_mouse_button_action(button, action, mode);
    }

    void key_callback(GLFWwindow* window, int keycode, int scancode, int action, int mode) {
        auto& wndCreator = wnd::get_system();

        inp::InputSystem& inpSys = inp::get_system();
        inpSys.keyboard.on_key_action(keycode, scancode, action, mode);
    }

    void window_refresh_callback(GLFWwindow* window) {
        auto& wndCreator = wnd::get_system();
        auto wnd = wndCreator.find_window({ window });
        if (auto swnd = wnd.lock()) {
            swnd->eventRefreshWindow(*swnd);
        }
    }

    void window_move_callback(GLFWwindow* window, int xpos, int ypos) {
        auto& wndCreator = wnd::get_system();
        auto wnd = wndCreator.find_window({ window });
        if (auto swnd = wnd.lock()) {
            swnd->eventRefreshWindow(*swnd);
        }
    }
}

windows::WindowSystem::WindowSystem()
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
}

windows::WindowSystem::~WindowSystem()
{
    glfwTerminate();
}

std::shared_ptr<windows::Window> windows::WindowSystem::make_window(std::string_view title, int width, int height)
{
    auto shared_window = std::make_shared<Window>(title, glm::ivec2{ width, height });

    if (!context) {
        context = std::make_unique<render::driver::gl::render_context>((GLADloadproc)glfwGetProcAddress);
    }

    auto window = shared_window->get_id();
    _windows[window] = shared_window;

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);
    glfwSetWindowRefreshCallback(window, window_refresh_callback);
    glfwSetWindowPosCallback(window, window_move_callback);
    return shared_window;
}

std::weak_ptr<windows::Window> windows::WindowSystem::find_window(Window::Id win)
{
    return _windows[win];
}

bool windows::WindowSystem::is_all_windows_close()
{
    for (const auto& [_, win] : _windows)
    {
        if (auto swin = win.lock()) {
            if (!swin->is_shutdown()) {
                return false;
            }
        }
    }
    return true;
}
