#pragma once
#include "ecs_entity.h"
#include <entt/entt.hpp>

namespace ecs
{
	extern entt::registry registry;
	entity create_entity();

	void remove_entity(entity ent);

	template<typename COMPONENT>
	void remove_component(entity ent) { registry.remove<COMPONENT>(ent); }
}

#include "ecs_common_system.hpp"