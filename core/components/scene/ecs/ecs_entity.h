#pragma once
#include <common.h>

namespace ecs
{
	class entity
	{
	public:
		entity(std::uint64_t idx)
			: index(idx) {};
		~entity() = default;
		entity(const entity&) = default;
		entity(entity&&) = default;
		entity& operator=(const entity&) = default;
		entity& operator=(entity&&) = default;
		operator std::size_t() const { return index; }

		auto operator<=>(const entity&) const = default;

		std::uint64_t index = 0;

		struct hasher {
			std::size_t operator()(const entity& ent) const {
				return ent;
			}
		};
	};
}