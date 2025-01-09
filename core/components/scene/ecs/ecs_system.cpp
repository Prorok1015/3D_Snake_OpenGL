#include "ecs_system.h"
#include "scn_camera_controller.h"
#include "scn_transform_system.h"

void ecs::process_systems(float time_second)
{
	scn::ecs_process_update_camera_matrix();
	scn::update_transform_system(time_second);
	scn::update_animation_system(time_second);
}
