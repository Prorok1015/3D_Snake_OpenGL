#pragma once
#include <common.h>
#include <scn_model.h>
#include <scn_camera_controller.h>
#include <ecs/ecs_entity.h>
#include <rnd_render_system.h>

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
		std::vector<std::string> cameras_list
		{
			"Main", "Second"
		};
		int current_camera = 0;

		std::vector<std::string> render_modes_list
		{
			"TRIANGLE", "TRIANGLE_STRIP", "LINE_LOOP", "LINE_STRIP", "LINE", "POINT"
		};

		std::unordered_map<std::string, rnd::RENDER_MODE> mmap{
				{render_modes_list[0], rnd::RENDER_MODE::TRIANGLE},
				{render_modes_list[1], rnd::RENDER_MODE::TRIANGLE_STRIP},
				{render_modes_list[2], rnd::RENDER_MODE::LINE_LOOP},
				{render_modes_list[3], rnd::RENDER_MODE::LINE_STRIP},
				{render_modes_list[4], rnd::RENDER_MODE::LINE},
				{render_modes_list[5], rnd::RENDER_MODE::POINT},
		};
		int current_render_mode = 0;

		std::vector<std::string> models_list
		{
			"objects/anim_cube_f/tail_cube.glb",
			"objects/robot/gen_robot.glb",
			"objects/anim_cube_f/bird_cube.glb",
			"objects/anim_cube_f/jump_cube.glb",
			"objects/anim_cube_f/anim_cube.glb",
			"objects/getaur/scene.gltf",
			"objects/train/scene.gltf",
			"objects/fsb/scene.gltf",
			"objects/backpack/backpack.obj",
			"objects/luke/Luke_01.fbx",
			"objects/flower/source/flower.fbx",
			"objects/helicopter/source/helicopter Space ship.glb",
			"objects/Cheeseburger.glb",
		};
		int current_model = 2;

		char buf[64] = "objects/";

		glm::vec4 clear_color {1};
		ecs::entity editor_web;
		ecs::entity light;
		ecs::entity sky;

		bool is_show_web = true;
		bool is_inited_ecs_test = false;
	};

}

namespace edt = editor;