#pragma once
#include "inp_input_manager_base.h"
#include "ecs_common_system.h"

namespace ecs
{
	class flow_input_manager : public inp::input_manager_base
	{
	public:
		flow_input_manager()
			: inp::input_manager_base("ecs", 10) {}

		virtual void on_notify_listeners(float dt) override;
		virtual bool on_handle_event(const inp::input_event&) override;

		template<class T>
		ecs::entity get_empty_entity() {
			for (const auto& ent : input_event_list) {
				if (!ecs::get_component<T>(ent)) {
					return ent;
				}
			}
			auto ent = ecs::create_entity();
			input_event_list.push_back(ent);
			return ent;
		}

	private:
		std::vector<ecs::entity> input_event_list;
	};
}