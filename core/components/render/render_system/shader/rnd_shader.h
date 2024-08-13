#pragma once
#include <common.h>
#include <rnd_shader_interface.h>

namespace render
{
	class Shader {
	public:
		Shader(driver::shader_interface* ptr) 
			: shader_impl(ptr) {}
		~Shader();

		void use();
		void uniform(std::string_view name, glm::mat4 matrix);
		void uniform(std::string_view name, int val);

	private:
		driver::shader_interface* shader_impl = nullptr;

	};
}
