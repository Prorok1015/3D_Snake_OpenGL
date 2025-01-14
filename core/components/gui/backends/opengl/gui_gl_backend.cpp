#include "gui_gl_backend.h"
#include "imgui.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"
#include "rnd_gl_texture.h"

void gui::gl::gl_imgui_backend::init(void* context)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(context), true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void gui::gl::gl_imgui_backend::new_frame()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    //ImGuiStyle& style = ImGui::GetStyle();
    //style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(1.0f, 0.1f, 0.1f, 0.0f);
    ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());
}

void gui::gl::gl_imgui_backend::render()
{
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void gui::gl::gl_imgui_backend::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void gui::gl::gl_imgui_backend::set_display_size(int width, int height)
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(width, height);
}

void gui::gl::gl_imgui_backend::set_input(float mouseX, float mouseY, bool mouseButtons[3], float mouseWheel) {
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2(mouseX, mouseY);
    for (int i = 0; i < 3; ++i)
        io.MouseDown[i] = mouseButtons[i];
    io.MouseWheel = mouseWheel;
}


ImTextureID gui::gl::gl_imgui_backend::get_imgui_texture_from_texture(rnd::driver::texture_interface* txm)
{
    if (!txm) {
        return 0;
    }

    auto* gl_txm = static_cast<rnd::driver::gl::texture*>(txm);

    return gl_txm->get_id();
}