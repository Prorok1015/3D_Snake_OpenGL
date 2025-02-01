#pragma once
#include <rnd_cubemap_interface.h>
#include <glad/glad.h>

namespace rnd::driver::gl
{
	class cubemap : public texture_interface
	{
	public:
		cubemap(GLuint id_, const texture_header& hdr)
			: id(id_) 
			, texture_interface(hdr)
		{}
		virtual ~cubemap() override;

		virtual void bind(unsigned int idx) override;

		virtual int width() const override { return 0; }
		virtual int height() const override { return 0; }
		virtual glm::ivec2 size() const override;

	private:
		GLuint id = 0;

	};
}