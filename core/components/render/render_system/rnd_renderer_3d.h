#pragma once
#include "common.h"
#include <scn_model.h>
#include <scn_mesh.h>
#include <rnd_vertex_array_interface.h>
#include <rnd_buffer_interface.h>
#include <camera/Camera.h>

namespace rnd
{
	class renderer_3d
	{
	public:
		renderer_3d(driver::driver_interface* drv);
		~renderer_3d();
		void init();
		void term();
		void draw(scene::Model& val);
		void draw(scene::Mesh& val);
		void set_camera(camera_static* cam) { camera = cam; }

	private:
		driver::driver_interface* drv = nullptr;
		camera_static* camera = nullptr;
		std::shared_ptr<driver::vertex_array_interface> vertex_array;
		std::shared_ptr<driver::buffer_interface> vertex_buffer;
		std::shared_ptr<driver::buffer_interface> index_buffer;
	};
}
