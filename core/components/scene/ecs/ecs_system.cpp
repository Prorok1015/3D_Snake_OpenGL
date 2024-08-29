#include "ecs_system.h"
#include "scn_camera_controller.h"

void ecs::process_systems()
{
	scn::ecs_process_update_camera_matrix();


}
