#pragma once

namespace render::driver
{
	class texture_interface
	{
	public:
		virtual ~texture_interface() {}

		virtual void bind() = 0;

		virtual int width() const = 0;
		virtual int height() const = 0;
	};
}