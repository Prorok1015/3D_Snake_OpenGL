#pragma once
#include <common/common.h>
#include <scene/model.h>

namespace editor
{
	class EditorSystem
	{
	public:
		EditorSystem();
		~EditorSystem();
		bool show_toolbar();

		void pre_render();

	private:
		//char buf[64] = "objects/getaur/scene.gltf";
		char buf[64] = "objects/backpack/backpack.obj";

		glm::vec4 clear_color_ {1};

		scene::Model g_Scene;
		bool is_draw_scene_net = true;
	};

}

namespace edt = editor;