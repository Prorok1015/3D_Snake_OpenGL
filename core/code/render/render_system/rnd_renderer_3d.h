#pragma once
#include "common.h"
#include <scn_model.h>
#include <scn_mesh.h>
#include <rnd_vertex_array_interface.h>
#include <rnd_buffer_interface.h>


namespace render
{
	class Renderer3d
	{
	public:
		Renderer3d();
		~Renderer3d();
		void init(driver::driver_interface* drv);
		void term();
		void draw(scene::Model& val);
		void draw(scene::Mesh& val);

	private:
		std::shared_ptr<driver::vertex_array_interface> vertex_array;
		std::shared_ptr<driver::buffer_interface> vertex_buffer;
		std::shared_ptr<driver::buffer_interface> index_buffer;
		driver::driver_interface* drv = nullptr;
	};
}

namespace rnd = render;