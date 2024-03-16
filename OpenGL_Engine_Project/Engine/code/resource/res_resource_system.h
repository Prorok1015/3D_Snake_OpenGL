#pragma once
#include "../common/common.h"
#include "res_resource_tag.h"

namespace resource
{
	class Resource;
	class Texture;
	class Shader;
	class Model;

	class ResourceSystem
	{
	public:
		ResourceSystem() = default;
		~ResourceSystem() = default;
		ResourceSystem(const ResourceSystem&) = delete;
		ResourceSystem& operator= (const ResourceSystem&) = delete;
		ResourceSystem(ResourceSystem&&) = default;
		ResourceSystem& operator= (ResourceSystem&&) = default;

		std::shared_ptr<Resource> require_resource(Tag tag);
		std::shared_ptr<resource::Texture> require_resource_texture(Tag tag);
		std::shared_ptr<resource::Shader> require_resource_shader(Tag tag);
		std::shared_ptr<resource::Model> require_resource_model(Tag tag);

	public:
		static std::string get_absolut_path(Tag tag);

	private:
		std::shared_ptr<Resource> find_cache(const Tag& tag);

	private:
		std::vector<std::shared_ptr<Resource>> cache_;

	};

	ResourceSystem& get_system();
}

namespace res = resource;