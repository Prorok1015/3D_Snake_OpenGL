#pragma once
#include "common.h"
#include "res_resource_tag.h"
#include "rnd_shader_interface.h"
#include "rnd_driver_interface.h"

namespace rnd
{
	struct shader_desc
	{
		struct shader_desc_hash {
			struct hasher {
				std::size_t operator() (const shader_desc_hash& desc) const noexcept {
					return desc.value;
				}
			};
			auto operator<=>(const shader_desc_hash& rhs) const noexcept = default;

			std::size_t value;
		};

		static shader_desc_hash get_hash(const shader_desc& desc) { return  shader_desc_hash{ std::hash<std::string>{}(desc.name) }; }

		bool operator== (const shader_desc& desc) const noexcept {
			return name == desc.name;
		}

		res::Tag tex0;
		res::Tag tex1;
		res::Tag tex2;
		res::Tag tex3;
		std::string name;
	};
	
	struct shader_scene_desc : public shader_desc
	{
		shader_scene_desc() {
			name = "scene";
		}
		glm::mat4 uWorldModel = glm::mat4{ 1.0 };
		glm::mat4 uWorldMeshMatr = glm::mat4{ 1.0 };
		int use_animation = 0;

		static std::vector<driver::shader_header> load();
		static void configure_render_pass(const shader_scene_desc& decs, rnd::driver::shader_interface* shader);
	};

	struct shader_sky_desc : public shader_desc
	{
		shader_sky_desc() {
			name = "sky";
		}
		static std::vector<driver::shader_header> load();
		static void configure_render_pass(const shader_sky_desc& decs, rnd::driver::shader_interface* shader);

		std::vector<res::Tag> cubemap0;
	};
}