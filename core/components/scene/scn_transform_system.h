#pragma once

namespace scn
{
	void update_transform_system(float time_second);
	void update_animation_system(float time_second);
	void update_bone_offsets_system(float time_second);
	void init_transform_system();
	void deinit_transform_system();
}