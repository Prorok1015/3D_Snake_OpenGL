#pragma once
#include <common.h>
#include <res_resource_tag.h>
#include "rnd_shader.h"

#include <rnd_driver_interface.h>
#include <rnd_uniform_buffer_interface.h>

namespace rnd
{
	struct GlobalUniform
	{
		glm::mat4 projection;
		glm::mat4 view;
		float time;
	};

	struct global_sun
	{
		glm::vec3 light_color;
		glm::vec3 position;
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

		void update_global_uniform(const GlobalUniform& val) const;
		void update_global_sun(const global_sun& val) const;
	protected:
		std::unique_ptr<rnd::driver::shader_interface> load(const std::string& tag) const;

	private:
		driver::driver_interface* drv = nullptr;
		std::shared_ptr<rnd::driver::uniform_buffer_interface> _matrices;
		std::shared_ptr<rnd::driver::uniform_buffer_interface> sun_light;
		mutable std::unordered_map<std::string_view, std::unique_ptr<rnd::driver::shader_interface>> _cache;
	};

}
