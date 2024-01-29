#include "window.h"
#include "../common/common.h"
#include "../common/enums.h"
//
//GLFWwindow* Window::window;
//int Window::width;
//int Window::height;
//float Window::lastTime = 0.f;
//float Window::delta = 0.f;
//unsigned Window::current_frame = 0;
using namespace application;

Event<std::function<void(GLFWwindow* window, int width, int height)>> resize_window_event;
Event<std::function<void(GLFWwindow* window, double xpos, double ypos)>> gCursor_position_window_event;
Event<std::function<void(GLFWwindow* window, int button, int action, int mode)>> gMouse_button_window_event;
Event<std::function<void(GLFWwindow* window, int keycode, int scancode, int action, int mode)>> gKeyboard_window_event;

void window_size_callback(GLFWwindow* window, int width, int height) {
    resize_window_event(window, width, height);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    gCursor_position_window_event(window, xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mode) {
    gMouse_button_window_event(window, button, action, mode);
}

void key_callback(GLFWwindow* window, int keycode, int scancode, int action, int mode) {
    gKeyboard_window_event(window, keycode, scancode, action, mode);
}

int Window::init_libraris()
{
    /* Initialize the library */
    glfwInit();
    return 0;
}

int Window::initialize(const char* title, int width, int height)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    resize_window_event += [this](GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
        width = width;
        height = height;
    };

    gCursor_position_window_event += [this](GLFWwindow* window, double xpos, double ypos) { this->cursor_position_window_event(window, xpos, ypos); };
    gMouse_button_window_event += [this](GLFWwindow* window, int button, int action, int mode) { this->mouse_button_window_event(window, button, action, mode); };
    gKeyboard_window_event += [this](GLFWwindow* window, int keycode, int scancode, int action, int mode) {this->keyboard_window_event(window, keycode, scancode, action, mode); };

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

    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

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
