#pragma once
#include <common.h>
#include <res_resource_tag.h>
#include "rnd_shader.h"

#include <rnd_driver_interface.h>
#include <rnd_gl_uniform_buffer.h>

namespace render
{
	struct GlobalUniform
	{
		glm::mat4 projection;
		glm::mat4 view;
		float time;
	};

	class ShaderManager
	{
	public:
		ShaderManager(driver::driver_interface* ptr);
		~ShaderManager();

		ShaderManager(const ShaderManager&) = delete;
		ShaderManager(ShaderManager&&) = delete;
		ShaderManager& operator= (const ShaderManager&) = delete;
		ShaderManager& operator= (ShaderManager&&) = delete;

		void clear_cache() const { _cache.clear(); }

		Shader use(std::string_view shader) const;
		void unuse() const;

		void uniform(const std::string_view shader, const std::string_view field, glm::mat4 val) const;
		void uniform(const std::string_view shader, const std::string_view field, int val) const;

		void init_global_uniform() const;
		void update_global_uniform(const GlobalUniform& val) const;
	protected:
		std::unique_ptr<render::driver::shader_interface> load(const std::string& tag) const;

	private:
		mutable std::unordered_map<std::string_view, std::unique_ptr<render::driver::shader_interface>> _cache;
		mutable std::shared_ptr<render::driver::gl::uniform_buffer> _matrices;
		driver::driver_interface* drv = nullptr;
	};

}

namespace rnd = render;
