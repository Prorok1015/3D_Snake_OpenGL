#pragma once
struct GLFWwindow;
enum class CursorMode;


class Window
{
public:	
	static GLFWwindow* window;
	static int width;
	static int height;
	static float lastTime;
	static float delta;
	static unsigned current_frame;

	static int initialize(const char* title, int width, int height);
	static void terminate();
	static void swap_buffers();
	static bool is_should_close();
	static void set_should_close(bool close = true);
	static void set_cursor_mode(CursorMode mode);

	static float current_time();
	static void update_frame();
};

