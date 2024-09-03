#pragma once
#include <rnd_cubemap_interface.h>
#include <glad/glad.h>

namespace rnd::driver::gl
{
	class cubemap : public cubemap_interface
	{
	public:
		cubemap(GLuint id_)
			: id(id_) {}
		virtual ~cubemap() override;

		virtual void bind() override;

		virtual int width() const override { return 0; }
		virtual int height() const override { return 0; }

	private:
		GLuint id = 0;
	};
}