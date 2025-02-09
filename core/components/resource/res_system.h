#pragma once
#include <common.h>
#include "res_tag.h"
#include <future>
#include "logger/engine_log.h"
#include "res_resource_base.h"

namespace res
{
	class ResourceSystem
	{
	public:
		static std::string get_absolut_path(const Tag& tag);

	public:
		ResourceSystem();
		~ResourceSystem() = default;
		ResourceSystem(const ResourceSystem&) = delete;
		ResourceSystem& operator= (const ResourceSystem&) = delete;
		ResourceSystem(ResourceSystem&&) = delete;
		ResourceSystem& operator= (ResourceSystem&&) = delete;

		template<class RESOURCE>
		std::future<std::shared_ptr<RESOURCE>> require_resource_async(Tag tag, bool hard_reload = false)
		{
			return std::async(std::launch::async, &ResourceSystem::require_resource<RESOURCE>, this, tag, hard_reload);
		}

		template<class RESOURCE>
		std::shared_ptr<RESOURCE> require_resource(const Tag& tag, bool hard_reload = false)
		{
			if (auto res = find_cache(tag)) {
				if (!hard_reload || tag.protocol() == Tag::memory) {
					return std::static_pointer_cast<RESOURCE>(res);
				}

				cache_.erase(std::remove(cache_.begin(), cache_.end(), res));
			}

			auto res = std::make_shared<RESOURCE>(tag);
			cache_.push_back(res);
			return res;
		}

		void add_resource(std::shared_ptr<res::Resource> resource)
		{
			if (std::find(cache_.begin(), cache_.end(), resource) != cache_.end()) {
				egLOG("resource/add", "Resource '{}' already exist!", resource->tag_.get_full());
				return;
			}

			cache_.push_back(resource);
		}

		bool is_exist(const Tag& tag) const {
			return find_cache(tag) != nullptr;
		}

		static std::filesystem::path get_resources_path() { return std::filesystem::path(s_res_path); }

	private:
		std::shared_ptr<Resource> find_cache(const Tag& tag) const;

	private:
		std::vector<std::shared_ptr<Resource>> cache_;
		static std::string s_res_path;

	};

	ResourceSystem& get_system();
}
