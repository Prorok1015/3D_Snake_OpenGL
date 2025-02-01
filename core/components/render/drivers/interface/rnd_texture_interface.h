#pragma once
#include "common.h"

namespace rnd::driver
{
	struct texture_header
	{
		enum class TYPE
		{
			R8,
			RGB8,
			RGBA8,
			R32I,
			D32F_S8,
			D24_S8,
			D32,
			D24,
			D16,
			D32F,
			S1,
			S4,
			S8
		};

		enum class FILTERING
		{
			NEAREST,
			LINEAR,
			LINEAR_MIPMAP
		};

		enum class WRAPPING
		{
			REPEAT,
			REPEAT_MIRROR,
			CLAMP_TO_EDGE,
			CLAMP_TO_BORDER
		};

		struct data
		{
			int width = 0;
			int height = 0;
			TYPE channels = TYPE::R8;
			unsigned char* data;
		};

		std::string name = "unknown";//TODO: maybe delete?
		data picture{};
		FILTERING min = FILTERING::LINEAR;
		FILTERING mag = FILTERING::LINEAR;
		WRAPPING wrap = WRAPPING::CLAMP_TO_EDGE;
	};


	class texture_interface
	{
	public:
		texture_interface(const texture_header& hdr)
			: header(hdr) {}
		virtual ~texture_interface() {}

		virtual void bind(unsigned int position) = 0;

		virtual int width() const = 0;
		virtual int height() const = 0;
		virtual glm::ivec2 size() const = 0;

		texture_header header;
	};
}