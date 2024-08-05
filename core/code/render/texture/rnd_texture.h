#pragma once
#include "../../common/common.h"
#include "../../resource/res_resource_tag.h"
//TODO
#include "open_gl_specific.h"
namespace render
{

	class Texture
	{
	public:
		enum class FILTERING
		{
			NEAREST = GL_NEAREST,
			LINEAR = GL_LINEAR,
			LINEAR_MIPMAP = GL_LINEAR_MIPMAP_LINEAR
		};

		enum class WRAPPING
		{
			REPEAT = GL_REPEAT,
			REPEAT_MIRROR = GL_MIRRORED_REPEAT,
			CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
			CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER
		};

		static std::shared_ptr<Texture> load(const res::Tag& tag);

	public:
		Texture(unsigned id, int width, int height) 
			: id(id), width_(width), height_(height) {
		}
		~Texture() {}

		void bind() {
			glBindTexture(GL_TEXTURE_2D, id);
			CHECK_GL_ERROR();
		}

		int width() const { return width_; }
		int height() const { return height_; }

		std::string tmp_type;

	protected:
		unsigned int id;
		int width_;
		int height_;
	};
}

namespace rnd = render;