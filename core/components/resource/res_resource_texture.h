#pragma once
#include <common.h>
#include "res_resource_base.h"

namespace res
{
	class Picture : public Resource
	{
	public:
		Picture(const Tag& tag);
		~Picture();

		unsigned char* data() const { return data_; }
		int channels() const { return channels_; }
		const glm::ivec2& size() const { return size_; }

	private:
		unsigned char* data_ = nullptr;
		int channels_ = 0;
		glm::ivec2 size_{ 0, 0 };
	};

	using ImageRef = std::shared_ptr<Picture>;
}
