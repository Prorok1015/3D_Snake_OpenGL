#pragma once
#include "common.h"

namespace ecs
{
	extern std::vector<std::function<void(float)>> systems;

	void process_systems(float time_second);
}