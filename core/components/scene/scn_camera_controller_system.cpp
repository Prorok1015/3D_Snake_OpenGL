#include "scn_camera_controller_system.h"
#include "scn_camera_controller_component.hpp"
#include "scn_camera_component.hpp"
#include "scn_model.h"
#include "inp_input_system.h"
#include "ecs_component.h"
#include "ecs_common_system.h"


bool is_point_in_rect_wh(glm::ivec2 point, glm::ivec4 rect) {
	return point.x >= rect.x &&
		point.x <= rect.x + rect.z &&
		point.y >= rect.y &&
		point.y <= rect.y + rect.w;
}

bool is_point_in_rect_tr(glm::ivec2 point, glm::ivec4 rect) {
	return point.x >= rect.x &&
		point.x <= rect.z &&
		point.y >= rect.y &&
		point.y <= rect.w;
}

// TODO: use dt
void scn::ecs_process_update_camera_matrix(const float time_second)
{
	auto events = ecs::filter<ecs::input_changed_event_component>();
		
	if (events.empty()) {
		return;
	}	
	const auto& evt = events.front();
	auto& input = inp::get_system();
	for (const auto& ent : ecs::filter<scn::camera_component, scn::mouse_controller_component>()) {

		auto* camera = ecs::get_component<scn::camera_component>(ent);
		auto* data = ecs::get_component<scn::mouse_controller_component>(ent);
		auto& rotation = data->rotation;
		auto& pos = data->position;
		auto& distance = data->distance;
		auto& speed = data->movement_speed;
		auto& is_moving = data->is_moving;
		auto& is_rotating = data->is_rotating;


		if (auto* mouse_btn = ecs::get_component<inp::mouse_click_event>(evt)) {
			auto* able_area = ecs::get_component<scn::input_area>(ent);
			if (mouse_btn->key == inp::MOUSE_BUTTONS::LEFT)
				is_moving = (mouse_btn->action == inp::KEY_ACTION::DOWN) 
				&& (able_area ? is_point_in_rect_wh(mouse_btn->pos, able_area->win_space_rect) : true);
			if (mouse_btn->key == inp::MOUSE_BUTTONS::RIGHT)
				is_rotating = (mouse_btn->action == inp::KEY_ACTION::DOWN)
				&& (able_area ? is_point_in_rect_wh(mouse_btn->pos, able_area->win_space_rect) : true);
		}

		if (auto* cursor = ecs::get_component<inp::cursor_move_event>(evt))	{
			glm::vec2 delta = (cursor->prev - cursor->pos) / glm::vec2(camera->viewport.size) * 2;

			if (is_moving)
			{
				glm::vec3 addition = glm::vec3(delta.x, 0, delta.y);
				pos = pos + (glm::rotateY(addition, rotation.y) * speed);
			}

			if (is_rotating)
			{
				float pitch = delta.y;
				float yaw = delta.x;

				rotation.x = std::clamp(rotation.x + pitch, -glm::radians(90.0f), glm::radians(90.0f));
				rotation.y += yaw;
			}
		}

		if (auto* scroll = ecs::get_component<inp::scroll_move_event>(evt)) {
			distance -= scroll->direction.y * speed;
			distance = std::clamp(distance, 0.f, 150.f);
		}

		if (auto* trans = ecs::get_component<scn::transform_component>(ent)) {
			glm::mat4 orientation = glm::toMat4(glm::quat(rotation));
			trans->local = glm::translate(pos) * orientation * glm::translate(glm::mat4(1.0), glm::vec3(0, 0, distance));
		}
	}
}