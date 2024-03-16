#include "res_resource_system.h"
#include "res_resource_texture.h"
#include "res_resource_shader.h"
#include "res_resource_model.h"

using namespace resource;

ResourceSystem* p_res_system = nullptr;

resource::ResourceSystem& resource::get_system()
{
	ASSERT_MSG(p_res_system, "Resource system is nullptr!");
	return *p_res_system;
}

std::shared_ptr<resource::Resource> resource::ResourceSystem::require_resource(Tag tag)
{
	return nullptr;
}

std::shared_ptr<resource::Texture> resource::ResourceSystem::require_resource_texture(Tag tag)
{
	if (auto res = find_cache(tag)) {
		return std::static_pointer_cast<res::Texture>(res);
	}
	auto res = std::make_shared<res::Texture>(tag);
	cache_.push_back(res);
	return res;
}

std::shared_ptr<resource::Shader> resource::ResourceSystem::require_resource_shader(Tag tag)
{
	return std::make_shared<res::Shader>(tag);
}

std::shared_ptr<resource::Model> resource::ResourceSystem::require_resource_model(Tag tag)
{
	return std::make_shared<res::Model>(tag);
}

std::string resource::ResourceSystem::get_absolut_path(Tag tag)
{
	std::string tmp_res_path = "D:/MyProject/SnakeProject/Editor/Editor/res/";
	if (tag.get_pref() == Tag::get_default_pref()) {
		std::string path{ tag.get_path() };
		std::string name{ tag.get_name() };
		return tmp_res_path + path + name;
	}

	egLOG("resource/absolut_path", "Broken tag {}", tag.get_full());
	return {};
}

std::shared_ptr<Resource> resource::ResourceSystem::find_cache(const Tag& tag)
{
	auto it = std::find_if(cache_.begin(), cache_.end(), [tag](auto res) { return *res == tag; });
	if (it != cache_.end()) {
		return *it;
	}

	return nullptr;
}
