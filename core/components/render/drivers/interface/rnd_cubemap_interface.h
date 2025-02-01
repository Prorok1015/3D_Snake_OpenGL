#pragma once
#include "rnd_texture_interface.h"

namespace rnd::driver
{
	struct cubmap_texture_header
	{
		texture_header::data right;
		texture_header::data left;
		texture_header::data bottom;
		texture_header::data top;
		texture_header::data front;
		texture_header::data back;

		texture_header::FILTERING min;
		texture_header::FILTERING mag;
		texture_header::WRAPPING wrap;
	};

	class cubemap_interface : public texture_interface
	{

	};
}