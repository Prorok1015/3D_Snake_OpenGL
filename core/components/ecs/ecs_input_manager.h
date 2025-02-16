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

		ecs::entity get_empty_entity() {
			if (!ecs::registry.valid(input_event)) {
				input_event = ecs::create_entity();
			}
			return input_event;
		}

	private:
		ecs::entity input_event = entt::null;
	};
}