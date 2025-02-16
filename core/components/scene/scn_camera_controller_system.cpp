#include "scn_camera_controller_system.h"
#include "scn_camera_controller_component.hpp"
#include "scn_camera_component.hpp"
#include "scn_model.h"
#include "inp_input_system.h"
#include "ecs_component.h"
#include "ecs_common_system.h"

namespace {
	struct is_mouse_controller_updated {};

	void on_update_mouse_controller_by_mouse_click_event(ecs::entity evt) {
		const auto& mouse_btn = ecs::registry.get<inp::mouse_click_event>(evt);

		for (const auto& [ent, data] : ecs::registry.view<scn::mouse_controller_component>().each()) {
			auto& is_moving = data.is_moving;
			auto& is_rotating = data.is_rotating;
			if (mouse_btn.key == inp::MOUSE_BUTTONS::LEFT)
				is_moving = (mouse_btn.action == inp::KEY_ACTION::DOWN);
			if (mouse_btn.key == inp::MOUSE_BUTTONS::RIGHT)
				is_rotating = (mouse_btn.action == inp::KEY_ACTION::DOWN);
			ecs::registry.emplace_or_replace<is_mouse_controller_updated>(ent);
		}
	}

	void on_update_mouse_controller_by_mouse_move_event(ecs::entity ent) {
		const auto& cursor = ecs::registry.get<inp::cursor_move_event>(ent);

		for (const auto& [ent, data] : ecs::registry.view<scn::mouse_controller_component>().each()) {
			auto& is_moving = data.is_moving;
			auto& is_rotating = data.is_rotating;
			auto& last_mouse_pos = data.last_mouse_pos;
			auto& pos = data.position;
			auto& rotation = data.rotation;
			auto& speed = data.movement_speed;
			auto& rotate_speed = data.rotating_speed;
			glm::vec2 delta = cursor.direction;
			last_mouse_pos = cursor.pos;

			if (is_moving)
			{
				glm::vec3 addition = glm::vec3(delta.x, 0, delta.y);
				pos += (glm::rotateY(addition * speed * 3, rotation.y));
			}

			if (is_rotating)
			{
				float pitch = delta.y * rotate_speed;
				float yaw = delta.x * rotate_speed;

				rotation.x = std::clamp(rotation.x + pitch, -glm::radians(90.0f), glm::radians(90.0f));
				rotation.y += yaw;
			}
			ecs::registry.emplace_or_replace<is_mouse_controller_updated>(ent);
		}
	}

	void on_update_mouse_controller_by_scroll_event(ecs::entity ent) {
		const auto& scroll = ecs::registry.get<inp::scroll_move_event>(ent);

		for (const auto& [ent, data] : ecs::registry.view<scn::mouse_controller_component>().each()) {
			auto& distance = data.distance;
			auto& speed = data.movement_speed;
			distance -= scroll.direction.y * speed;
			distance = std::clamp(distance, 0.f, 150.f);
			ecs::registry.emplace_or_replace<is_mouse_controller_updated>(ent);
		}
	}

	void on_update_mouse_controller_local_transform(ecs::entity ent) {
		if (ecs::registry.all_of<scn::local_transform, scn::mouse_controller_component>(ent)) {
			auto& trans = ecs::registry.get<scn::local_transform>(ent);
			auto& data = ecs::registry.get<scn::mouse_controller_component>(ent);
			auto& rotation = data.rotation;
			auto& pos = data.position;
			auto& distance = data.distance;
			glm::mat4 orientation = glm::toMat4(glm::quat(rotation));
			trans.local = glm::translate(pos) * orientation * glm::translate(glm::mat4(1.0), glm::vec3(0, 0, distance));
		}
	}
}

void scn::init_camera_controller_system() {
	entt::sigh_helper{ecs::registry}
	.with<inp::mouse_click_event>()
	.on_construct<&on_update_mouse_controller_by_mouse_click_event>()
	.on_update<&on_update_mouse_controller_by_mouse_click_event>()
	.with<inp::cursor_move_event>()
	.on_construct<&on_update_mouse_controller_by_mouse_move_event>()
	.on_update<&on_update_mouse_controller_by_mouse_move_event>()
	.with<inp::scroll_move_event>()
	.on_construct<&on_update_mouse_controller_by_scroll_event>()
	.on_update<&on_update_mouse_controller_by_scroll_event>()
	.with<is_mouse_controller_updated>()
	.on_construct<&on_update_mouse_controller_local_transform>()
	.on_update<&on_update_mouse_controller_local_transform>();
}

void scn::deinit_camera_controller_system() {
	ecs::registry.on_construct<inp::mouse_click_event>().disconnect<&on_update_mouse_controller_by_mouse_click_event>();
	ecs::registry.on_update<inp::mouse_click_event>().disconnect<&on_update_mouse_controller_by_mouse_click_event>();
	ecs::registry.on_construct<inp::cursor_move_event>().disconnect<&on_update_mouse_controller_by_mouse_move_event>();
	ecs::registry.on_update<inp::cursor_move_event>().disconnect<&on_update_mouse_controller_by_mouse_move_event>();
	ecs::registry.on_construct<inp::scroll_move_event>().disconnect<&on_update_mouse_controller_by_scroll_event>();
	ecs::registry.on_update<inp::scroll_move_event>().disconnect<&on_update_mouse_controller_by_scroll_event>();
	ecs::registry.on_construct<is_mouse_controller_updated>().disconnect<&on_update_mouse_controller_local_transform>();
}
