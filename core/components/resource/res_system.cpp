#include "res_system.h"
#include "res_picture.h"
#include "res_resource_text_file.h"
#include "res_resource_model.h"
#include <engine_log.h>
#include <boost/json.hpp>

using namespace res;

ResourceSystem* p_res_system = nullptr;
std::string ResourceSystem::s_res_path = RESOURCE_PATH;

res::ResourceSystem& res::get_system()
{
	ASSERT_MSG(p_res_system, "Resource system is nullptr!");
	return *p_res_system;
}

res::ResourceSystem::ResourceSystem()
{
	if (s_res_path.empty()) {
		ASSERT_MSG(std::filesystem::exists("./res/"), "The './res/' folder should be exist next to your exe");
		s_res_path = std::filesystem::absolute("./res/").generic_string();
	}
}

std::string res::ResourceSystem::get_absolut_path(const Tag& tag)
{
	if (tag.protocol() == Tag::default_protocol()) {
		std::string path{ tag.path() };
		std::string name{ tag.name() };
		return s_res_path + path + name;
	}

	egLOG("resource/absolut_path", "Broken tag {}", tag.get_full());
	return std::string{};
}

std::shared_ptr<Resource> res::ResourceSystem::find_cache(const Tag& tag) const
{
	auto it = std::find_if(cache_.begin(), cache_.end(), [&tag](auto res) { return *res == tag; });
	if (it != cache_.end()) {
		return *it;
	}

	return nullptr;
}
