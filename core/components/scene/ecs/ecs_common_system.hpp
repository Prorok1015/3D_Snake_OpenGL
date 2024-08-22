#include <common.h>
#include <unordered_map>
#include <ds_type_id.hpp>

namespace ecs
{
	std::uint64_t get_new_index() {
		static std::uint64_t next_index = 0;
		return ++next_index;
	}	
	
	entity create_entity() {
		return entity(get_new_index());
	}

	std::unordered_map<ds::Type::unique_id, std::vector<char>>& get_pool() {
		static std::unordered_map<ds::Type::unique_id, std::vector<char>> pool;
		return pool;
	}

	std::unordered_map<entity, std::vector<std::pair<int, std::size_t>>, entity::hasher>& get_entitis() {
		static std::unordered_map<entity, std::vector<std::pair<int, std::size_t>>, entity::hasher> entitis;
		return entitis;
	}


	template<typename COMPONENT>
	bool is_able_component(entity ent) {
		ds::Type::unique_id com_id = ds::Type::value<COMPONENT>();

		for (auto& [com, offset] : get_entitis()[ent]) {
			if (com == com_id) {
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

		ds::Type::unique_id com_id = ds::Type::value<COMPONENT>();

		std::size_t component_offset = 0;
		for (auto& [com, offset] : get_entitis()[ent]) {
			if (com_id == com) {
				component_offset = offset;
				break;
			}
		}

		std::vector<char>& components = get_pool()[com_id];
		return static_cast<COMPONENT*>((void*)(components.data() + component_offset));
	}

	template<typename COMPONENT>
	COMPONENT* add_component(entity ent, COMPONENT component) {
		ds::Type::unique_id com_id = ds::Type::value<COMPONENT>();
		std::vector<char>& components = get_pool()[com_id];
		
		constexpr std::size_t component_size = sizeof(COMPONENT);
		const std::size_t component_offset = components.size();

		char* pointer = static_cast<char*>((void*)(&component));
		for (std::size_t i = 0; i < component_size; ++i) {
			components.push_back(pointer[i]);
		}

		get_entitis()[ent].push_back({ com_id, component_offset });

		return static_cast<COMPONENT*>((void*)(components.data() + component_offset));
	}

	template<typename COMPONENT>
	std::vector<entity> filter() {
		ds::Type::unique_id com_id = ds::Type::value<COMPONENT>();
		std::vector<entity> result;

		for (auto& [ent, components] : get_entitis()) {
			for (auto& [com, offset] : components) {
				if (com == com_id) {
					result.push_back(ent);
					break;
				}
			}
		}

		return result;
	}
}