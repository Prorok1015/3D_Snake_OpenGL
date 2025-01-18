#pragma once
#include <common.h>

namespace scn
{
	struct mouse_controller_component
	{
		float movement_speed = 15.f;
		float distance = 20;
		glm::vec3 rotation{ 0 };
		glm::vec3 position{ 0 };
		bool is_moving = false;
		bool is_rotating = false;
	};

	struct input_area
	{
		glm::ivec4 win_space_rect{ 0 };
	};

}