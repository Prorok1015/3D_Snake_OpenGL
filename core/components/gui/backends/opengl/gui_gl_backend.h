#pragma once
#include "gui_backend_interface.h"
#include "imgui.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

namespace gui::gl
{
    class gl_imgui_backend : public gui::imgui_backend_interface {
    public:
        void init(void* context) override {
            // Setup Dear ImGui context
            IMGUI_CHECKVERSION();
            ImGui::CreateContext();
            ImGuiIO& io = ImGui::GetIO(); (void)io;
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
            io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

            ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(context), true);
            ImGui_ImplOpenGL3_Init("#version 330 core");
        }

        void new_frame() override {
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
        }

        void render() override {
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        }

        void shutdown() override {
            ImGui_ImplOpenGL3_Shutdown();
            ImGui_ImplGlfw_Shutdown();
            ImGui::DestroyContext();
        }

        void set_display_size(int width, int height) override {
            ImGuiIO& io = ImGui::GetIO();
            io.DisplaySize = ImVec2(width, height);
        }

        void set_input(float mouseX, float mouseY, bool mouseButtons[3], float mouseWheel) override {
            ImGuiIO& io = ImGui::GetIO();
            io.MousePos = ImVec2(mouseX, mouseY);
            for (int i = 0; i < 3; ++i)
                io.MouseDown[i] = mouseButtons[i];
            io.MouseWheel = mouseWheel;
        }

    private:
    };
}