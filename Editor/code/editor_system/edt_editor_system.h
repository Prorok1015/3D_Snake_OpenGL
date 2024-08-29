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

		glm::vec4 clear_color_ {1};
		ecs::entity editor_web;

		bool is_show_web = true;
		bool is_inited_ecs_test = false;

		std::shared_ptr<rnd::camera> camera;
		std::shared_ptr<rnd::camera> second_camera;
		std::shared_ptr<scn::mouse_camera_controller> camera_controller;
		std::shared_ptr<scn::mouse_camera_controller> second_camera_controller;
	};

}

namespace edt = editor;