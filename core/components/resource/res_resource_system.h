#pragma once
#include <common.h>
#include "res_resource_tag.h"

namespace resource
{
	class Resource;

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
		std::shared_ptr<RESOURCE> require_resource(const Tag& tag, bool hard_reload = false)
		{
			if (auto res = find_cache(tag)) {
				if (!hard_reload) {
					return std::static_pointer_cast<RESOURCE>(res);
				}

				cache_.erase(std::remove(cache_.begin(), cache_.end(), res));
			}

			auto res = std::make_shared<RESOURCE>(tag);
			cache_.push_back(res);
			return res;
		}

	private:
		std::shared_ptr<Resource> find_cache(const Tag& tag);

	private:
		std::vector<std::shared_ptr<Resource>> cache_;

	};

	ResourceSystem& get_system();
}

namespace res = resource;