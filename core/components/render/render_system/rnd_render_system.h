#pragma once
#include <common.h>
#include <rnd_driver_interface.h>

#include "texture/rnd_texture_manager.h"
#include "shader/rnd_shader_manager.h"
#include "rnd_renderer_3d.h"

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

		void enable(driver::ENABLE_FLAGS flag) const {
			drv->enable(flag);
		}
		void clear_color(const glm::vec4& val) const {
			drv->set_clear_color(val);
		}
		void clear(driver::CLEAR_FLAGS flag) const { drv->clear(flag); }
		void activate_texture_unit(int i) const {
			drv->set_activate_texture(i);
		}

		void reset_texture() const {
			drv->set_activate_texture(0);
		} 

		void set_point_size(float size) const {
			drv->set_point_size(size);
		}

		void set_line_size(float size) const {
			drv->set_line_size(size);
		}

		void draw_elements(unsigned int vao, unsigned int count) const {
			if (get_render_mode() == RENDER_MODE::POINT) {
				set_point_size(10.f);
			}

			drv->draw_elements(get_render_mode(), vao, count);
		}

		void draw_elements(unsigned int count) const {
			if (get_render_mode() == RENDER_MODE::POINT) {
				set_point_size(10.f);
			}

			drv->draw_elements(get_render_mode(), count);
		}

		void registrate_renderer(std::weak_ptr<renderer_base> renderer);
		void unregistrate_renderer(std::weak_ptr<renderer_base> renderer);

		void produce_renderers() const;

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
}
