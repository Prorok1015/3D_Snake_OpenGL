#pragma once
#include <rnd_shader_interface.h>
#include <glad/glad.h>
#include <glm/ext/matrix_float4x4.hpp>

namespace rnd::driver::gl
{
	class shader : public rnd::driver::shader_interface
	{
	public:
		shader(GLuint shader_id) : id(shader_id) {}
		virtual ~shader() override;

		virtual void uniform(std::string_view name, glm::mat4 val) override;
		virtual void uniform(std::string_view name, glm::vec4 val) override;
		virtual void uniform(std::string_view name, glm::vec3 val) override;
		virtual void uniform(std::string_view name, glm::vec2 val) override;
		virtual void uniform(std::string_view name, int val) override;
		virtual void uniform(std::string_view name, float val) override;
		virtual void use() override;

	private:
		GLuint id = 0;
	};
}