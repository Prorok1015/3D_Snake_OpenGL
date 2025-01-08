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
		virtual ~renderer_3d() override;

		virtual void on_render(rnd::driver::driver_interface* drv);

		void draw(scn::Model& val, rnd::driver::driver_interface* drv);
		void draw(rnd::shader_scene_desc& desc, res::Mesh& mesh, rnd::driver::driver_interface* drv);
		void draw(rnd::shader_scene_desc& desc, res::mesh_view& mesh, const res::meshes_conteiner& data, rnd::driver::driver_interface* drv);
		void draw_line(rnd::driver::driver_interface* drv);
		void draw_instances(rnd::driver::driver_interface* drv);
		void draw_sky(rnd::driver::driver_interface* drv);
		void draw_model(rnd::driver::driver_interface* drv);
		void draw_ecs_model(rnd::driver::driver_interface* drv);
		void draw_ecs_meshes(ecs::entity ent, const res::meshes_conteiner& data, rnd::shader_scene_desc& scene, rnd::driver::driver_interface* drv);
		void draw_hierarchy(res::meshes_conteiner& data, rnd::shader_scene_desc& shader, glm::mat4& model_world, res::node_hierarchy_view& hir, glm::mat4 parent, rnd::driver::driver_interface* drv);
		void setup_instance_buffer();

	public:
		bool is_flag_test_render = true;
		bool is_flag_show_anim = true;
		long long start_time = 0;
		std::vector<ecs::entity> scene_objects;
		std::unique_ptr<rnd::driver::vertex_array_interface> vertex_array;
		std::shared_ptr<rnd::driver::buffer_interface> vertex_buffer;
		//std::shared_ptr<rnd::driver::buffer_interface> bones_buffer;
		std::shared_ptr<rnd::driver::buffer_interface> index_buffer;

		std::unique_ptr<rnd::driver::vertex_array_interface> vertex_array_inst;
		std::shared_ptr<rnd::driver::buffer_interface> matrices_buffer_inst;
		std::shared_ptr<rnd::driver::buffer_interface> vertex_buffer_inst;
		std::shared_ptr<rnd::driver::buffer_interface> index_buffer_inst;
	};

}