#include "ecs_common_system.h"

namespace ecs {
	entt::registry registry;
}

ecs::entity ecs::create_entity() { return registry.create(); } 

void ecs::remove_entity(entity ent)  { registry.destroy(ent); }