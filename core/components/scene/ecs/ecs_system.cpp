#include "ecs_system.h"

namespace ecs {
	std::vector<std::function<void(float)>> systems;
}

void ecs::process_systems(float time_second)
{
	for (auto& system : systems)
	{
		system(time_second);
	}

}
