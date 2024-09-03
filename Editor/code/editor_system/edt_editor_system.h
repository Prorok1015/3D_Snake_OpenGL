#pragma once
#include <common.h>
#include <scn_model.h>
#include <scn_camera_controller.h>
#include <ecs/ecs_entity.h>

namespace editor
{
	class EditorSystem
	{
	public:
		EditorSystem();
		~EditorSystem();
		bool show_toolbar();

		//TODO change to renderer
		bool show_web();

		bool show_ecs_test();

		bool init_ecs_test();
	private:
		//char buf[64] = "objects/getaur/scene.gltf";
		char buf[64] = "objects/backpack/backpack.obj";
		//char buf[64] = "objects/helicopter/source/helicopter Space ship.glb";
		//char buf[64] = "objects/Cheeseburger.glb";

		glm::vec4 clear_color {1};
		ecs::entity editor_web;
		ecs::entity light;
		ecs::entity sky;

		bool is_show_web = true;
		bool is_inited_ecs_test = false;

		std::shared_ptr<rnd::camera> camera;
		std::shared_ptr<rnd::camera> second_camera;
		std::shared_ptr<scn::mouse_camera_controller> camera_controller;
		std::shared_ptr<scn::mouse_camera_controller> second_camera_controller;
	};

}

namespace edt = editor;