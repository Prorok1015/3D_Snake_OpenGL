#pragma once
#include <glm/glm.hpp>
#include <string_view>

namespace rnd::driver
{
	class shader_interface
	{
	public:
		virtual ~shader_interface() {}

		virtual void uniform(std::string_view name, glm::mat4 val) = 0;
		virtual void uniform(std::string_view name, int val) = 0;
		virtual void uniform(std::string_view name, float val) = 0;
		virtual void use() = 0;
	};
}