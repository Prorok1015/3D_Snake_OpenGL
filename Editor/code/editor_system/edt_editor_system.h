#pragma once
#include <common.h>
#include <scn_model.h>
#include <scn_camera_controller.h>

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

	private:
		//char buf[64] = "objects/getaur/scene.gltf";
		char buf[64] = "objects/backpack/backpack.obj";

		glm::vec4 clear_color_ {1};

		scn::Model g_Scene;
		bool is_draw_scene_net = true;
		std::shared_ptr<scn::mouse_camera_controller> camera_controller;
	};

}

namespace edt = editor;