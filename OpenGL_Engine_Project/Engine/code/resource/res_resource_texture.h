#pragma once
#include "../common/common.h"
#include "res_resource_base.h"

namespace resource
{
	class Texture : public Resource
	{
	public:
		Texture(Tag tag);
		~Texture();

		unsigned char* data() const { return data_; }
		int channels() const { return channels_; }
		const glm::vec2& size() const { return size_; }

	private:
		unsigned char* data_ = nullptr;
		int channels_ = 0;
		glm::vec2 size_;
	};

}

namespace res = resource;