#include "debug_ui_renderer.h"
#include "imgui.h"

#include "backends/imgui_impl_glfw.h"
#include "backends/imgui_impl_opengl3.h"

dbg_ui::renderer::renderer()
	: rnd::renderer_base(0)
{
}

void dbg_ui::renderer::on_render(rnd::driver::driver_interface* drv)
{
	// Start the Dear ImGui frame
	//TODO: maybe Call once on init?
	ImGui_ImplOpenGL3_NewFrame();
	//TODO: move to input update step
	ImGui_ImplGlfw_NewFrame();
	//TODO: this too
	ImGui::NewFrame();

	render_event();

	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
