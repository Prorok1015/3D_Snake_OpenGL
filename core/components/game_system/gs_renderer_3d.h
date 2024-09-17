#pragma once
#include <rnd_renderer_base.h>
#include <scn_model.h>
#include <camera/rnd_camera.h>
#include <ecs/ecs_entity.h>
#include <res_resource_model.h>

namespace rnd::driver
{
	class driver_interface;
	class vertex_array_interface;
	class buffer_interface;
}

namespace gs
{
	class renderer_3d : public rnd::renderer_base
	{
	public:
		renderer_3d();
		virtual ~renderer_3d() override {}

		virtual void on_render(rnd::driver::driver_interface* drv);

		void draw(scn::Model& val, rnd::driver::driver_interface* drv);
		void draw(res::Mesh& mesh, rnd::driver::driver_interface* drv);
		void draw_line(rnd::driver::driver_interface* drv);
	public:
		std::vector<ecs::entity> scene_objects;
		std::shared_ptr<rnd::driver::vertex_array_interface> vertex_array;
		std::shared_ptr<rnd::driver::buffer_interface> vertex_buffer;
		std::shared_ptr<rnd::driver::buffer_interface> index_buffer;
	};

}