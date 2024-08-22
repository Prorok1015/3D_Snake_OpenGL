#pragma once
#include "ecs_entity.h"

namespace ecs
{
	entity create_entity();

	template<typename COMPONENT>
	bool is_able_component(entity ent);

	template<typename COMPONENT>
	COMPONENT* get_component(entity ent);

	template<typename COMPONENT>
	COMPONENT* add_component(entity ent, COMPONENT component);

	template<typename COMPONENT>
	std::vector<entity> filter();
}

#include "ecs_common_system.hpp"