#pragma once

namespace gui
{
    class imgui_backend_interface {
    public:
        virtual ~imgui_backend_interface() {}

        virtual void init(void* context = nullptr) = 0;
        virtual void new_frame() = 0;
        virtual void render() = 0;
        virtual void shutdown() = 0;
        virtual void set_display_size(int width, int height) = 0;
        virtual void set_input(float mouseX, float mouseY, bool mouseButtons[3], float mouseWheel) = 0;
    };
}