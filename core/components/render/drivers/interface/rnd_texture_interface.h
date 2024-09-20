#pragma once

namespace rnd::driver
{
	enum class texture_type
	{
		RED8,
		RGB8,
		RGBA8
	};

	class texture_interface
	{
	public:
		virtual ~texture_interface() {}

		virtual void bind(unsigned int position) = 0;

		virtual int width() const = 0;
		virtual int height() const = 0;
	};
}