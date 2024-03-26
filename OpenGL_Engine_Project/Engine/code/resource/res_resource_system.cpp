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

resource::ResourceSystem::ResourceSystem()
{
	ASSERT_MSG(std::filesystem::exists("./res/"), "You should have folder './res/' near by your exe.");
}

std::string resource::ResourceSystem::get_absolut_path(const Tag& tag)
{
	std::string tmp_res_path = std::filesystem::absolute("./res/").generic_string();

	if (tag.protocol() == Tag::default_protocol()) {
		std::string path{ tag.path() };
		std::string name{ tag.name() };
		return tmp_res_path + path + name;
	}

	egLOG("resource/absolut_path", "Broken tag {}", tag.get_full());
	ASSERT_FAIL("Invalid tag");
	return {};
}

std::shared_ptr<Resource> resource::ResourceSystem::find_cache(const Tag& tag)
{
	auto it = std::find_if(cache_.begin(), cache_.end(), [&tag](auto res) { return *res == tag; });
	if (it != cache_.end()) {
		return *it;
	}

	return nullptr;
}
