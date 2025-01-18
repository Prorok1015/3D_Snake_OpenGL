#include "ecs_common_system.h"

namespace ecs {
	std::uint64_t get_new_index() {
		static std::uint64_t next_index = 0;
		return ++next_index;
	}

	std::unordered_map<ds::Type::unique_id, std::vector<std::byte>>& get_pool() {
		static std::unordered_map<ds::Type::unique_id, std::vector<std::byte>> pool;
		return pool;
	}

	std::unordered_map<entity, std::vector<std::pair<ds::Type::unique_id, std::size_t>>, entity::hasher>& get_entitis() {
		static std::unordered_map<entity, std::vector<std::pair<ds::Type::unique_id, std::size_t>>, entity::hasher> entitis;
		return entitis;
	}
}

ecs::entity ecs::create_entity() {
	return entity(ecs::get_new_index());
}

//TODO: delete component from pool and recalculate all components offset
void ecs::remove_entity(entity ent) {

	auto& entity_mask = get_entitis()[ent];
	entity_mask.clear();
}
