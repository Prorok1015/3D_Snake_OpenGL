#pragma once
#include "../common/common.h"
#include "texture/rnd_texture_manager.h"
#include "shader/rnd_shader_manager.h"
#include "rnd_renderer_3d.h"

#include <rnd_driver_interface.h>

namespace render
{
	using RENDER_MODE = driver::RENDER_MODE;

	class RenderSystem
	{
	public:
		RenderSystem(std::unique_ptr<render::driver::driver_interface> driver);
		~RenderSystem() = default;
		RenderSystem(const RenderSystem&) = delete;
		RenderSystem(RenderSystem&&) = delete;
		RenderSystem& operator= (const RenderSystem&) = delete;
		RenderSystem& operator= (RenderSystem&&) = delete;

		void init();
		void term();

		const rnd::TextureManager& get_txr_manager() const { return txrManager; }
		const rnd::ShaderManager& get_sh_manager() const { return shManager; }
		rnd::Renderer3d& get_renderer() { return renderer; }

		void viewport(glm::ivec4 rect) const {
			pDrv->set_viewport(rect[0], rect[1], rect[2], rect[3]);
		}

		void enable(int flag) const {
			pDrv->enable(flag);
		}
		void clear_color(const glm::vec4& val) const {
			pDrv->set_clear_color(val.r, val.g, val.b, val.a);
		}
		void clear(int flag) const { pDrv->clear(flag); }
		void activate_texture_unit(int i) const {
			pDrv->set_activate_texture(i);
		}

		void reset_texture() const {
			pDrv->set_activate_texture(0);
		} 

		void set_point_size(float size) const {
			pDrv->set_point_size(size);
		}

		void set_line_size(float size) const {
			pDrv->set_line_size(size);
		}

		void draw_elements(unsigned int vao, unsigned int count) const {
			if (render_mode() == RENDER_MODE::POINT) {
				set_point_size(10.f);
			}

			pDrv->draw_elements(render_mode(), vao, count);
		}

		void draw_elements(unsigned int count) const {
			if (render_mode() == RENDER_MODE::POINT) {
				set_point_size(10.f);
			}

			pDrv->draw_elements(render_mode(), count);
		}

		RENDER_MODE render_mode() const { return _render_mode; }
		void render_mode(RENDER_MODE val) { _render_mode = val; }
	private:
		std::unique_ptr<render::driver::driver_interface> pDrv = nullptr;
		rnd::TextureManager txrManager;
		rnd::ShaderManager shManager;
		rnd::Renderer3d renderer;

		RENDER_MODE _render_mode = RENDER_MODE::TRIANGLE;
	};

	RenderSystem& get_system();
}

namespace rnd = render;