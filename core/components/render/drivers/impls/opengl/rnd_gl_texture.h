#pragma once
#include <rnd_texture_interface.h>
#include <glad/glad.h>

namespace rnd::driver::gl
{
	class texture : public rnd::driver::texture_interface
	{
	public:
		texture(GLuint id, int w, int h); 
		virtual ~texture() override {}

		virtual void bind() override;

		virtual int width() const override;
		virtual int height() const override;

	private:
		GLuint id = 0;
		int width_i = 0;
		int height_i = 0;
	};
}