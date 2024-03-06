#pragma once
#include "../common/common.h"

namespace resource
{
	class Resource;

	class ResourceSystem
	{
	public:
		std::shared_ptr<Resource> require_resource();

	private:
		std::vector<std::shared_ptr<Resource>> cache_;
	};

}

namespace res = resource;