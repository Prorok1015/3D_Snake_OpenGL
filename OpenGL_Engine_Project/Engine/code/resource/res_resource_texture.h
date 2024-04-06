#pragma once
#include "../common/common.h"
#include "res_resource_base.h"

namespace resource
{
	class Image : public Resource
	{
	public:
		Image(const Tag& tag);
		~Image();

		unsigned char* data() const { return data_; }
		int channels() const { return channels_; }
		const glm::ivec2& size() const { return size_; }

	private:
		unsigned char* data_ = nullptr;
		int channels_ = 0;
		glm::ivec2 size_{ 0, 0 };
	};

}

namespace res = resource;