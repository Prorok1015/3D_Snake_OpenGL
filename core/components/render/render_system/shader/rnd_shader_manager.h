#pragma once
#include <common.h>
#include <res_resource_tag.h>
#include "rnd_shader.h"
#include "rnd_scene_shader_desc.h"

#include <rnd_driver_interface.h>
#include <rnd_uniform_buffer_interface.h>

namespace rnd
{
	struct global_params
	{
		glm::mat4 projection;
		glm::mat4 view;
		float time;
		glm::vec4 view_position;
	};

	struct light_point
	{
		glm::vec4 position;
		glm::vec4 diffuse;
		glm::vec4 ambient;
		glm::vec4 specular;
	};
	

	struct bones_matrices
	{
		static constexpr int MAX_BONE_MATRICES_COUNT = 128;
		int row_height;
		int bone_count;
	private:
		glm::ivec2 _std140padding;
	public:
		glm::mat4 bones[MAX_BONE_MATRICES_COUNT];
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

		void clear_cache() const { _cache.clear(); _cache_new.clear();}

		Shader use(std::string_view shader) const;
		Shader use(const char* shader) const { return use(std::string_view(shader)); }
		void unuse() const;

		template<class T>
		void use(const T& desc) const
		{
			auto it = _cache_new.find(shader_desc::get_hash(desc));
			rnd::driver::shader_interface* shader = nullptr;
			if (it == _cache_new.end()) {
				auto new_shader = drv->create_shader(T::load());
				if (new_shader) {
					shader = new_shader.get();
					_cache_new[shader_desc::get_hash(desc)] = std::move(new_shader);
				}
			} else {
				shader = it->second.get();
			}

			if (shader) {
				shader->use();
				T::configure_render_pass(desc, shader);
			}
		}

		void uniform(const std::string_view shader, const std::string_view field, glm::mat4 val) const;
		void uniform(const std::string_view shader, const std::string_view field, int val) const;

		void update_global_uniform(const global_params& val) const;
		void update_global_sun(const light_point& val) const;
		void update_global_bones_matrices(const bones_matrices& val, std::size_t count) const;
	protected:
		std::unique_ptr<rnd::driver::shader_interface> load(const std::string& tag) const;

	private:
		driver::driver_interface* drv = nullptr;
		std::shared_ptr<rnd::driver::uniform_buffer_interface> _matrices;
		std::shared_ptr<rnd::driver::uniform_buffer_interface> sun_light;
		std::shared_ptr<rnd::driver::uniform_buffer_interface> bones_buffer;
		mutable std::unordered_map<std::string_view, std::unique_ptr<rnd::driver::shader_interface>> _cache;
		mutable std::unordered_map<shader_desc::shader_desc_hash, std::unique_ptr<rnd::driver::shader_interface>, shader_desc::shader_desc_hash::hasher> _cache_new;
	};

}
