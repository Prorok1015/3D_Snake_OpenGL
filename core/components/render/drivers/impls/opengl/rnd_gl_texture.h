#pragma once
#include <rnd_texture_interface.h>
#include <glad/glad.h>

namespace rnd::driver::gl
{
	class texture : public rnd::driver::texture_interface
	{
	public:
		texture(GLuint id, const texture_header& hdr); 
		virtual ~texture() override;

		virtual void bind(unsigned int position) override;

		virtual int width() const override;
		virtual int height() const override;
		virtual glm::ivec2 size() const override;

		GLuint get_id() const { return id; }
	private:
		GLuint id = 0;
		int width_i = 0;
		int height_i = 0;
	};
}