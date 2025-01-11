#include "scn_camera_controller.h"
#include <ecs/ecs_common_system.h>
#include "scn_model.h"
#include "inp_input_system.h"
#include "ecs/ecs_component.h"
#include "scn_camera_component.hpp"

// TODO: use dt
void scn::ecs_process_update_camera_matrix(const float time_second)
{
	auto& input = inp::get_system();

	for (auto ent : ecs::filter<scn::camera_component, scn::mouse_controller_component>()) {
		auto event_ = ecs::filter<ecs::input_changed_event_component>();
		
		if (event_.empty()) {
			return;
		}
		auto* camera = ecs::get_component<scn::camera_component>(ent);
		auto* data = ecs::get_component<scn::mouse_controller_component>(ent);
		auto& rotation = data->rotation;
		auto& pos = data->position;
		auto& distance = data->distance;
		auto& speed = data->movement_speed;

		glm::vec2 delta = input.mouse.get_delta_pos() / glm::vec2(camera->viewport.size) * 2;

		if (input.get_key_state(inp::MOUSE_BUTTONS::LEFT).action == inp::KEY_ACTION::DOWN)
		{
			glm::vec3 addition = glm::vec3(delta.x, 0, delta.y);
			pos = pos + (glm::rotateY(addition, rotation.y) * speed);
		}

		if (input.get_key_state(inp::MOUSE_BUTTONS::RIGHT).action == inp::KEY_ACTION::DOWN)
		{
			float pitch = delta.y;
			float yaw = delta.x;

			rotation.x = std::clamp(rotation.x + pitch, -glm::radians(90.0f), glm::radians(90.0f));
			rotation.y += yaw;
		}

		distance -= input.mouse.get_scroll().y * speed;
		distance = std::clamp(distance, 0.f, 150.f);

		if (auto* trans = ecs::get_component<scn::transform_component>(ent)) {
			glm::mat4 orientation = glm::toMat4(glm::quat(rotation));
			trans->local = glm::translate(pos) * orientation * glm::translate(glm::mat4(1.0), glm::vec3(0, 0, distance));
		}
	}
}