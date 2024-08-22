#pragma once
#ifndef WINDOW_CAMERA_H_
#define WINDOW_CAMERA_H_

#include <eng_transform_3d.hpp>
#include <common.h>

namespace rnd {
	class camera {
	public:
		camera(glm::vec3 position, glm::ivec4 viewport, float fov = 90.0f);
		virtual ~camera() {};

		glm::mat4 projection() const;
		virtual glm::mat4 view() const;

		void set_viewport(glm::ivec2 offset, glm::ivec2 size);
		glm::ivec4 get_viewport() const { return glm::ivec4{ viewport_offset, viewport_size }; }

		void set_enabled(bool enable) { is_enabled = enable; }
		bool get_is_enabled() const { return is_enabled; }

		void set_visible_distance(float dist) { visible_distance = std::clamp(dist, MIN_VISIBLE_DISTANCE, std::abs(dist)); }
		float get_visible_distance() const { return visible_distance; }

		float get_aspect() const { 
			return (float)viewport_size.x / (float)viewport_size.y; 
		}

		glm::mat4 world;
	public:
		eng::transform3d transform;
		glm::ivec2 viewport_offset{ 0 };
		glm::ivec2 viewport_size{ 0 };

		float fov = 90.f;
		float visible_distance = 150.f;

		bool is_enabled = false;

		static constexpr float MIN_VISIBLE_DISTANCE = 0.1f;
	};
}
#endif /* WINDOW_CAMERA_H_ */


