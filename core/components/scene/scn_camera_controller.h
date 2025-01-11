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
	};

	void ecs_process_update_camera_matrix(const float time_second);
}