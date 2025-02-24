#include "wnd_window_system.h"
#include "wnd_window.h"
#include "inp_input_system.h"
#include "rnd_gl_render_context.h"
#include "gui_gl_backend.h"
#include "wnd_input_keycode_convert.hpp"
#include <filesystem>

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
        inp::InputSystem& inpSys = inp::get_system();
        inpSys.mouse.on_window_resize({ width, height });
    }

    void device_scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
        inp::InputSystem& inpSys = inp::get_system();
        inpSys.mouse.on_mouse_scroll(xoffset, yoffset);
        inp::scroll_move_event evt{ .direction = { xoffset, yoffset } };
        inpSys.on_scroll_move_event(evt);
    }

    void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
        auto& wndCreator = wnd::get_system();

        inp::InputSystem& inpSys = inp::get_system();
        inpSys.mouse.on_mouse_move(xpos, ypos);
        inp::cursor_move_event evt{ .pos = {xpos, ypos}, .prev = inpSys.mouse.get_old_pos(), .direction = inpSys.mouse.get_direction() };
        inpSys.on_cursor_move_event(evt);
    }

    void mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
        if (button == -1 || action == GLFW_REPEAT) {
            return;
        }

        auto key_optional = wnd::convert::to_mouse_keycode(button);
        if (!key_optional.has_value()) {
            return;
        }
        inp::InputSystem& inpSys = inp::get_system();

        inp::mouse_click_event evt;
        evt.key = key_optional.value();
        evt.action = wnd::convert::to_action(action);
        evt.pos = inpSys.mouse.get_pos();
        inpSys.on_mouse_buttons_event(evt);
        inpSys.mouse.on_mouse_button_action(evt.key, evt.action, mode);

    }

    void key_callback(GLFWwindow* window, int keycode, int scancode, int action, int mode) {
        if (keycode == -1 || action == GLFW_REPEAT) {
            return;
        }

        auto key_optional = wnd::convert::to_keyboard_keycode(keycode);
        if (!key_optional.has_value()) {
            return;
        }

        inp::InputSystem& inpSys = inp::get_system();

        inp::keyboard_event evt;
        evt.key = key_optional.value();
        evt.action = wnd::convert::to_action(action);
        inpSys.on_keyboard_event(evt);
        inpSys.keyboard.on_key_action(evt.key, scancode, evt.action, mode);
   }

    void window_refresh_callback(GLFWwindow* window) {
        auto& wndCreator = wnd::get_system();
        if (auto wnd = wndCreator.find_window({ window })) {
            //wnd->eventRefreshWindow(*wnd);
        }
    }

    void window_move_callback(GLFWwindow* window, int xpos, int ypos) {
        auto& wndCreator = wnd::get_system(); 
        if (auto wnd = wndCreator.find_window({ window })) {
            //wnd->eventRefreshWindow(*wnd);
        }
    }
}

wnd::WindowSystem::WindowSystem()
{
    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    // TODO: move to gl context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

    active_window = make_window()->get_id();   
    context = std::make_unique<rnd::driver::gl::render_context>((GLADloadproc)glfwGetProcAddress);
    imgui_backend = std::make_unique<gui::gl::gl_imgui_backend>();
    imgui_backend->init(active_window.internal_id);
    auto is_exist = std::filesystem::exists(imgui_backend->get_settings_filename());
    imgui_backend->set_initial_layout_by_default(!is_exist);
}

wnd::WindowSystem::~WindowSystem()
{
    imgui_backend->shutdown();
    glfwTerminate();
}

std::shared_ptr<wnd::window> wnd::WindowSystem::make_window()
{
    auto shared_window = std::make_shared<window>(wnd::context{ title });

    window::short_id wid = shared_window->get_id();
    windows_list[wid] = shared_window;

    glfwSetKeyCallback(wid, key_callback);
    glfwSetMouseButtonCallback(wid, mouse_button_callback);
    glfwSetScrollCallback(wid, device_scroll_callback);
    glfwSetCursorPosCallback(wid, cursor_position_callback);

    glfwSetWindowRefreshCallback(wid, window_refresh_callback);
    glfwSetWindowPosCallback(wid, window_move_callback);
    glfwSetWindowSizeCallback(wid, window_size_callback);

    inp::get_system().mouse.on_window_resize(shared_window->get_size());

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

void wnd::WindowSystem::pool_events() const
{
    glfwPollEvents();
}


void wnd::WindowSystem::init_windows_frame_time() const
{
    for (const auto& [_, win] : windows_list) {
        win->init_frame();
    }
}

void wnd::WindowSystem::process_windows() const
{
    for (const auto& [_, win] : windows_list) {
        win->update_frame();
        win->on_update();
    }
}

bool wnd::WindowSystem::is_stop_running()
{
    for (const auto& [_, win] : windows_list)
    { 
        if (!win->is_shutdown()) {
            return false;
        } 
    }
    return true;
}
