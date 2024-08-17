#include "wnd_window_system.h"
#include "wnd_window.h"
#include <inp_input_system.h>
#include <rnd_gl_render_context.h>

wnd::WindowSystem* p_wnd_system = nullptr;

wnd::WindowSystem& wnd::get_system()
{
    ASSERT_MSG(p_wnd_system, "Window system is nullptr!");
    return *p_wnd_system;
}

namespace {
    void window_size_callback(GLFWwindow* window, int width, int height) {
        auto& wndCreator = wnd::get_system();
        if (auto wnd = wndCreator.find_window({ window })) {
            wnd->on_resize_window(width, height);
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
        if (auto wnd = wndCreator.find_window({ window })) {
            wnd->eventRefreshWindow(*wnd);
        }
    }

    void window_move_callback(GLFWwindow* window, int xpos, int ypos) {
        auto& wndCreator = wnd::get_system(); 
        if (auto wnd = wndCreator.find_window({ window })) {
            wnd->eventRefreshWindow(*wnd);
        }
    }
}

wnd::WindowSystem::WindowSystem()
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    active_window = make_window()->get_id();   
    context = std::make_unique<render::driver::gl::render_context>((GLADloadproc)glfwGetProcAddress);
}

wnd::WindowSystem::~WindowSystem()
{
    glfwTerminate();
}

std::shared_ptr<wnd::window> wnd::WindowSystem::make_window()
{
    auto shared_window = std::make_shared<window>(wnd::context{ title });

    window::short_id wid = shared_window->get_id();
    windows_list[wid] = shared_window;

    glfwSetKeyCallback(wid, key_callback);
    glfwSetWindowSizeCallback(wid, window_size_callback);
    glfwSetMouseButtonCallback(wid, mouse_button_callback);
    glfwSetCursorPosCallback(wid, cursor_position_callback);
    glfwSetWindowRefreshCallback(wid, window_refresh_callback);
    glfwSetWindowPosCallback(wid, window_move_callback);
    return shared_window;
}

std::shared_ptr<wnd::window> wnd::WindowSystem::get_active_window()
{
    return find_window(active_window);
}

std::shared_ptr<wnd::window> wnd::WindowSystem::find_window(window::short_id win)
{
    return windows_list[win];
}

bool wnd::WindowSystem::is_all_windows_close()
{
    for (const auto& [_, win] : windows_list)
    { 
        if (!win->is_shutdown()) {
            return false;
        } 
    }
    return true;
}
