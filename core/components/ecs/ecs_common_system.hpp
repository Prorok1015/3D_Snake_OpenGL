#pragma once
#include <common.h>
#include <unordered_map>
#include <ds_type_id.hpp>

namespace ecs
{
	/*
	std::unordered_map<ds::Type::unique_id, std::vector<std::byte>>& get_pool();
	std::unordered_map<entity, std::vector<std::pair<ds::Type::unique_id, std::size_t>>, entity::hasher>& get_entitis();

	//TODO: separate to two funcs
	template<typename COMPONENT>
	bool is_able_component(entity ent) {
		ds::Type::unique_id able_id = ds::Type::value<COMPONENT>();

		for (auto& [com_id, offset] : get_entitis()[ent]) {
			if (com_id == able_id) {
				return true;
			}
		}

		return false;
	}

	template<typename COMPONENT>
	COMPONENT* get_component(entity ent) {
		if (!is_able_component<COMPONENT>(ent)) {
			return nullptr;
		}

		ds::Type::unique_id finding_id = ds::Type::value<COMPONENT>();

		std::size_t component_offset = 0;
		//TODO: make faster
		for (auto& [com_id, offset] : get_entitis()[ent]) {
			if (com_id == finding_id) {
				component_offset = offset;
				break;
			}
		}

		std::vector<std::byte>& components = get_pool()[finding_id];
		return static_cast<COMPONENT*>((void*)(components.data() + component_offset));
	}

	template<typename COMPONENT>
	COMPONENT* add_component(entity ent, COMPONENT component) {
		if (is_able_component<COMPONENT>(ent)) {
			ASSERT_FAIL("Component '{}' already added to entity '{}'", ds::Type::value<COMPONENT>(), ent.index);
			return get_component<COMPONENT>(ent);
		}

		ds::Type::unique_id component_id = ds::Type::value<COMPONENT>();
		std::vector<std::byte>& component_pool = get_pool()[component_id];
		
		const std::size_t new_component_offset = component_pool.size();
		component_pool.resize(component_pool.size() + sizeof(COMPONENT));
		new((void*)(component_pool.data() + new_component_offset)) COMPONENT(std::move(component));

		get_entitis()[ent].push_back({ component_id, new_component_offset });

		return static_cast<COMPONENT*>((void*)(component_pool.data() + new_component_offset));
	}

	//TODO: implement func
	template<typename COMPONENT>
	void remove_component(entity ent) {
		ds::Type::unique_id com_id = ds::Type::value<COMPONENT>();
		auto& components = get_pool()[com_id];
		auto& entity_mask = get_entitis()[ent];

		//TODO: delete component from pool and recalculate all components offset
		constexpr std::size_t component_size = sizeof(COMPONENT);

		entity_mask.erase(
			std::remove_if(entity_mask.begin(), entity_mask.end(), 
				[com_id](const auto& par) { return par.first == com_id; })
		);
	}

	template<typename... COMPONENTS>
	std::vector<entity> filter() {
		std::vector<ds::Type::unique_id> components_ids{ ds::Type::value<COMPONENTS>()... };
		std::vector<entity> result;

		//TODO: make faster
		for (auto& [ent, components] : get_entitis()) {
			std::size_t count = components_ids.size();
			for (auto& able_id : components_ids) {
				for (auto& [com_id, _] : components) {
					if (able_id == com_id) {
						--count;
						break;
					}
				}
			}

			if (count == 0) {
				result.push_back(ent);
			}
		}

		return result;
	}
	*/
}