#pragma once
#include "res_resource_model.h"

namespace res
{
	class instance_object
	{
	public:
		res::Mesh tpl;
		std::vector<glm::mat4> worlds;
	};
}