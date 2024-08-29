#pragma once
#include <common.h>
#include <rnd_driver_interface.h>

#include "texture/rnd_texture_manager.h"
#include "shader/rnd_shader_manager.h"

#include "rnd_renderer_base.h"

namespace rnd
{
	using RENDER_MODE = driver::RENDER_MODE;

	class RenderSystem
	{
	public:
		RenderSystem(std::unique_ptr<rnd::driver::driver_interface> driver);
		~RenderSystem() = default;
		RenderSystem(const RenderSystem&) = delete;
		RenderSystem(RenderSystem&&) = delete;
		RenderSystem& operator= (const RenderSystem&) = delete;
		RenderSystem& operator= (RenderSystem&&) = delete;
		
		const rnd::TextureManager& get_texture_manager() const { return texture_manager; }
		const rnd::ShaderManager& get_shader_manager() const { return shader_manager; }

		void activate_renderer(std::weak_ptr<renderer_base> renderer);
		void deactivate_renderer(std::weak_ptr<renderer_base> renderer);

		void process_renderers() const;

		RENDER_MODE get_render_mode() const { return render_mode; }
		void set_render_mode(RENDER_MODE val) { render_mode = val; }

		rnd::driver::driver_interface* get_driver() const { return drv.get(); }
	private:
		std::unique_ptr<rnd::driver::driver_interface> drv = nullptr;
		rnd::TextureManager texture_manager;
		rnd::ShaderManager shader_manager;

		std::vector<std::weak_ptr<renderer_base>> renderers_list;

		RENDER_MODE render_mode = RENDER_MODE::TRIANGLE;
	};

	RenderSystem& get_system();

	struct render_mode_component {
		RENDER_MODE mode;
	};
}
