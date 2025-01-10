#pragma once
#include "inp_input_manager_base.h"
#include "inp_keyboard_device.h"
#include "inp_mouse_device.h"
#include "ecs_common_system.h"

namespace ecs
{
	class flow_input_manager : public inp::input_manager_base
	{
	public:
		flow_input_manager() = default;

		virtual void on_notify_listeners(float dt) override;

		inp::KeyboardDevice keyboard_prev;
		inp::MouseDevice mouse_prev;

		ecs::entity input_ent = ecs::create_entity();
	};
}