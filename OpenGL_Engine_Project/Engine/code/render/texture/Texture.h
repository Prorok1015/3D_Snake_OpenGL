#pragma once
#include "../../common/common.h"
#include <string_view>
#include <memory>
#include "../../resource/res_resource_texture.h"

class Texture {
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

public:
	Texture(unsigned int id, int width, int height);
	Texture(const Texture&) = default;//delete;
	~Texture();
	void bind();

	static Texture load_from_res(std::shared_ptr<res::Texture> res);
	static std::shared_ptr<Texture> load(std::string_view filename);
	static std::shared_ptr<Texture> load(std::string_view filename, FILTERING filtering, WRAPPING wrapping);
	static Texture loadClean(std::string_view filename, FILTERING minfiltering, FILTERING magfiltering, WRAPPING wrapping);
public:
	int width() const { return width_; }
	int height() const { return height_; }

	std::string type;
	std::string path;
private:
	unsigned int id;
	int width_;
	int height_;
};
